#include "Precompiled.h"
#include "EditorWidgets.h"
#include "Common.h"
#include "WidgetTypes.h"
#include "GroupMessage.h"
#include "BackwardCompatibilityManager.h"
#include "WidgetSelectorManager.h"

template <> tools::EditorWidgets* MyGUI::Singleton<tools::EditorWidgets>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::EditorWidgets>::mClassTypeName("EditorWidgets");

namespace tools
{
	const std::string LogSection = "LayoutEditor";

	EditorWidgets::EditorWidgets() :
		mWidgetsChanged(false)
	{
	}

	EditorWidgets::~EditorWidgets()
	{
		destroyAllSectors();
	}

	void EditorWidgets::initialise()
	{
		mWidgetsChanged = true;

		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("IgnoreParameters") = MyGUI::newDelegate(this, &EditorWidgets::loadIgnoreParameters);
		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("SkinReplace") = MyGUI::newDelegate(this, &EditorWidgets::loadSkinReplace);

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &EditorWidgets::notifyFrameStarted);
		MyGUI::WidgetManager::getInstance().registerUnlinker(this);
	}

	void EditorWidgets::shutdown()
	{
		MyGUI::WidgetManager::getInstance().unregisterUnlinker(this);
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &EditorWidgets::notifyFrameStarted);

		destroyAllWidgets();
		destroyAllSectors();
	}

	void EditorWidgets::destroyAllWidgets()
	{
		for (std::vector<WidgetContainer*>::iterator iter = mWidgets.begin(); iter != mWidgets.end(); ++iter)
			delete *iter;
		mWidgets.clear();
	}

	bool EditorWidgets::load(const MyGUI::UString& _fileName)
	{
		size_t index = _fileName.find("|");
		if (index != MyGUI::UString::npos)
		{
			MyGUI::UString fileName = _fileName.substr(0, index);
			MyGUI::UString itemIndex = _fileName.substr(index + 1);

			return loadFromProject(fileName, MyGUI::utility::parseValue<size_t>(itemIndex));
		}

		mCurrentFileName = _fileName;
		mCurrentItemName.clear();

		MyGUI::xml::Document doc;
		if (!doc.open(_fileName))
		{
			MYGUI_LOGGING(LogSection, Error, getClassTypeName() << " : " << doc.getLastError());
			return false;
		}

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if ((nullptr == root) || (root->getName() != "MyGUI"))
		{
			MYGUI_LOGGING(LogSection, Error, getClassTypeName() << " : '" << _fileName << "', tag 'MyGUI' not found");
			return false;
		}

		if (root->findAttribute("type") == "Layout")
		{
			loadWidgetsFromXmlNode(root);
		}
		else
		{
			return false;
		}

		mWidgetsChanged = true;
		return true;
	}

	bool EditorWidgets::loadFromProject(const MyGUI::UString& _fileName, size_t _index)
	{
		mCurrentFileName = _fileName;

		MyGUI::xml::Document doc;
		if (!doc.open(_fileName))
		{
			MYGUI_LOGGING(LogSection, Error, getClassTypeName() << " : " << doc.getLastError());
			return false;
		}

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if ((nullptr == root) || (root->getName() != "MyGUI"))
		{
			MYGUI_LOGGING(LogSection, Error, getClassTypeName() << " : '" << _fileName << "', tag 'MyGUI' not found");
			return false;
		}

		if (root->findAttribute("type") == "Resource")
		{
			// ����� ����� � ��������
			MyGUI::xml::ElementEnumerator element = root->getElementEnumerator();
			while (element.next("Resource"))
			{
				if (element->findAttribute("type") == "ResourceLayout")
				{
					if (_index == 0)
					{
						mCurrentItemName = element->findAttribute("name");

						loadWidgetsFromXmlNode(element.current());

						break;
					}
					else
					{
						_index --;
					}
				}
			}
		}
		else
		{
			return false;
		}

		mWidgetsChanged = true;
		return true;
	}

	bool EditorWidgets::save(const MyGUI::UString& _fileName)
	{
		size_t index = _fileName.find("|");
		if (index != MyGUI::UString::npos)
		{
			MyGUI::UString fileName = _fileName.substr(0, index);
			MyGUI::UString itemIndex = _fileName.substr(index + 1);

			return saveToProject(fileName, MyGUI::utility::parseValue<size_t>(itemIndex));
		}

		mCurrentFileName = _fileName;
		mCurrentItemName.clear();

		MyGUI::xml::Document doc;
		doc.createDeclaration();
		MyGUI::xml::ElementPtr root = doc.createRoot("MyGUI");
		root->addAttribute("type", "Layout");

		saveWidgetsToXmlNode(root, true);

		if (!doc.save(_fileName))
		{
			MYGUI_LOGGING(LogSection, Error, getClassTypeName() << " : " << doc.getLastError());
			return false;
		}

		return true;
	}

	bool EditorWidgets::saveToProject(const MyGUI::UString& _fileName, size_t _index)
	{
		mCurrentFileName = _fileName;

		MyGUI::xml::Document doc;
		if (!doc.open(_fileName))
		{
			MYGUI_LOGGING(LogSection, Error, getClassTypeName() << " : " << doc.getLastError());
			return false;
		}

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if ((nullptr == root) || (root->getName() != "MyGUI"))
		{
			MYGUI_LOGGING(LogSection, Error, getClassTypeName() << " : '" << _fileName << "', tag 'MyGUI' not found");
			return false;
		}

		if (root->findAttribute("type") == "Resource")
		{
			// ����� ����� � ��������
			MyGUI::xml::ElementEnumerator element = root->getElementEnumerator();
			while (element.next("Resource"))
			{
				if (element->findAttribute("type") == "ResourceLayout")
				{
					if (_index == 0)
					{
						mCurrentItemName = element->findAttribute("name");

						element->clear();
						element->addAttribute("type", "ResourceLayout");
						element->addAttribute("name", mCurrentItemName);

						saveWidgetsToXmlNode(element.current(), true);

						if (!doc.save(_fileName))
						{
							MYGUI_LOGGING(LogSection, Error, getClassTypeName() << " : " << doc.getLastError());
							return false;
						}

						return true;
					}
					else
					{
						_index --;
					}
				}
			}
			return false;
		}

		return false;
	}

	void EditorWidgets::loadxmlDocument(MyGUI::xml::Document* doc, bool _testMode)
	{
		MyGUI::xml::ElementPtr root = doc->getRoot();

		std::string type;
		if (root->findAttribute("type", type))
		{
			if (type == "Layout")
			{
				loadWidgetsFromXmlNode(root, _testMode);
			}
		}
		mWidgetsChanged = true;
	}

	MyGUI::xml::Document* EditorWidgets::savexmlDocument()
	{
		MyGUI::xml::Document* doc = new MyGUI::xml::Document();

		doc->createDeclaration();
		MyGUI::xml::ElementPtr root = doc->createRoot("MyGUI");
		root->addAttribute("type", "Layout");

		saveWidgetsToXmlNode(root);

		return doc;
	}

	void EditorWidgets::add(WidgetContainer* _container)
	{
		if (nullptr == _container->widget->getParent())
		{
			//if ("" == _container->getLayerName())
				//_container->layer = DEFAULT_LAYER;
			mWidgets.push_back(_container);
		}
		else
		{
			MyGUI::Widget* parent = _container->widget->getParent();
			WidgetContainer* containerParent = find(parent);
			while (nullptr == containerParent)
			{
				parent = parent->getParent();
				if (parent == nullptr)
					return;
				containerParent = find(parent);
			}
			containerParent->childContainers.push_back(_container);
		}
		mWidgetsChanged = true;
	}

	void EditorWidgets::remove(MyGUI::Widget* _widget)
	{
		remove(find(_widget));
		mWidgetsChanged = true;
	}

	void EditorWidgets::remove(WidgetContainer* _container)
	{
		std::vector<WidgetContainer*>::reverse_iterator iter;
		while (_container->childContainers.empty() == false)
		{
			iter = _container->childContainers.rbegin();
			remove(*iter);
		}

		if (nullptr != _container)
		{
			if (nullptr == _container->widget->getParent())
			{
				mWidgets.erase(std::find(mWidgets.begin(), mWidgets.end(), _container));
			}
			else
			{
				MyGUI::Widget* parent = _container->widget->getParent();
				WidgetContainer* containerParent = find(parent);
				while (nullptr == containerParent)
				{
					parent = parent->getParent();
					if (parent == nullptr)
					{
						// ������ �� ������ ����, ��� ������ ������,
						// ��� ��� ������� ������ �� ���������� ��� ���������� ������ �������
						return;
					}
					containerParent = find(parent);
				}

				containerParent->childContainers.erase(std::find(containerParent->childContainers.begin(), containerParent->childContainers.end(), _container));
			}

			MyGUI::Gui::getInstance().destroyWidget(_container->widget);

			delete _container;
		}
		mWidgetsChanged = true;
	}

	bool EditorWidgets::unbind(WidgetContainer* _container)
	{
		bool result = false;

		std::vector<WidgetContainer*>::reverse_iterator iter;
		while (_container->childContainers.empty() == false)
		{
			iter = _container->childContainers.rbegin();
			if (unbind(*iter))
				result = true;
		}

		if (nullptr != _container)
		{
			if (nullptr == _container->widget->getParent())
			{
				mWidgets.erase(std::find(mWidgets.begin(), mWidgets.end(), _container));
			}
			else
			{
				MyGUI::Widget* parent = _container->widget->getParent();
				WidgetContainer* containerParent = find(parent);
				while (nullptr == containerParent)
				{
					parent = parent->getParent();
					if (parent == nullptr)
					{
						// ������ �� ������ ����, ��� ������ ������,
						// ��� ��� ������� ������ �� ���������� ��� ���������� ������ �������
						return result;
					}
					containerParent = find(parent);
				}

				containerParent->childContainers.erase(std::find(containerParent->childContainers.begin(), containerParent->childContainers.end(), _container));
			}

			delete _container;
			result = true;
		}

		mWidgetsChanged = true;
		return result;
	}

	void EditorWidgets::clear()
	{
		mCurrentFileName.clear();
		mCurrentItemName.clear();

		while (!mWidgets.empty())
		{
			remove(mWidgets[mWidgets.size()-1]);
		}

		destroyAllSectors();
	}

	WidgetContainer* EditorWidgets::find(MyGUI::Widget* _widget)
	{
		return _find(_widget, "", mWidgets);
	}

	WidgetContainer* EditorWidgets::find(const std::string& _name)
	{
		return _find(nullptr, _name, mWidgets);
	}

	WidgetContainer* EditorWidgets::_find(MyGUI::Widget* _widget, const std::string& _name, std::vector<WidgetContainer*> _widgets)
	{
		for (std::vector<WidgetContainer*>::iterator iter = _widgets.begin(); iter != _widgets.end(); ++iter)
		{
			if (((*iter)->widget == _widget) || ((_name.empty() == false) && ((*iter)->name == _name)))
			{
				return *iter;
			}
			WidgetContainer* retContainer = _find(_widget, _name, (*iter)->childContainers);
			if (retContainer)
				return retContainer;
		}
		return nullptr;
	}

	void EditorWidgets::parseWidget(MyGUI::xml::ElementEnumerator& _widget, MyGUI::Widget* _parent, bool _testMode)
	{
		WidgetContainer* container = new WidgetContainer();
		// ������ �������� �������
		MyGUI::IntCoord coord;
		MyGUI::Align align = MyGUI::Align::Default;
		MyGUI::WidgetStyle widgetStyle = MyGUI::WidgetStyle::Child;
		std::string position;

		_widget->findAttribute("name", container->name);
		_widget->findAttribute("type", container->type);
		_widget->findAttribute("skin", container->skin);
		container->setLayerName(_widget->findAttribute("layer"));
		if (_widget->findAttribute("style", container->style))
			widgetStyle = MyGUI::WidgetStyle::parse(container->style);
		if (_widget->findAttribute("align", container->align))
			align = MyGUI::Align::parse(container->align);
		if (_widget->findAttribute("position", position))
			coord = MyGUI::IntCoord::parse(position);
		if (_widget->findAttribute("position_real", position))
		{
			container->relative_mode = true;
			//FIXME ������ ����� ���� � �� �������
			coord = MyGUI::CoordConverter::convertFromRelative(MyGUI::FloatCoord::parse(position), _parent == nullptr ? MyGUI::RenderManager::getInstance().getViewSize() : _parent->getClientCoord().size());
		}

		// ��������� ���� �� �����������
		std::string skin = container->skin;
		bool exist = isSkinExist(container->skin);
		if (!exist && !container->skin.empty())
		{
			skin = WidgetTypes::getInstance().findWidgetStyle(container->type)->default_skin;

			std::string tmp;
			if (skin.empty())
				tmp = "empty skin";
			else
				tmp = "'" + skin + "'";

			// FIXME : not translated string
			std::string mess = MyGUI::utility::toString("'", container->skin, "' skin not found , temporary changed to ", tmp);
			GroupMessage::getInstance().addMessage(mess, MyGUI::LogLevel::Error);
		}

		if (!_testMode)
			skin = getSkinReplace(skin);

		std::string layer = _testMode ? DEFAULT_LAYER : DEFAULT_EDITOR_LAYER;

		if (nullptr == _parent)
		{
			container->widget = MyGUI::Gui::getInstance().createWidgetT(container->type, skin, coord, align, layer);
		}
		else
		{
			container->widget = _parent->createWidgetT(widgetStyle, container->type, skin, coord, align, layer);
		}

		add(container);

		// ����� ����� � ��������
		MyGUI::xml::ElementEnumerator widget = _widget->getElementEnumerator();
		while (widget.next())
		{
			std::string key, value, type;

			if (widget->getName() == "Widget")
			{
				parseWidget(widget, container->widget, _testMode);
			}
			else if (widget->getName() == "Property")
			{
				// ������ ��������
				if (!widget->findAttribute("key", key))
					continue;
				if (!widget->findAttribute("value", value))
					continue;

				// ��������� �������� � ������� ������� ������
				key = MyGUI::BackwardCompatibility::getPropertyRename(key);
				size_t indexSeparator = key.find('_');
				if (indexSeparator != std::string::npos)
					key = key.substr(indexSeparator + 1);

				// � �������� ������� ��������
				if (tryToApplyProperty(container->widget, key, value, _testMode) == false)
					continue;

				container->mProperty.push_back(MyGUI::PairString(key, value));
			}
			else if (widget->getName() == "UserString")
			{
				// ������ ��������
				if (!widget->findAttribute("key", key))
					continue;
				if (!widget->findAttribute("value", value))
					continue;
				//container->mUserString.insert(MyGUI::PairString(key, value));
				container->mUserString.push_back(MyGUI::PairString(key, value));
			}
			else if (widget->getName() == "Controller")
			{
				// ������ ��������
				if (!widget->findAttribute("type", type))
					continue;
				ControllerInfo* controllerInfo = new ControllerInfo();
				controllerInfo->mType = type;

				MyGUI::ControllerItem* item = nullptr;
				if (_testMode)
				{
					item = MyGUI::ControllerManager::getInstance().createItem(type);
				}

				MyGUI::xml::ElementEnumerator prop = widget->getElementEnumerator();
				while (prop.next("Property"))
				{
					if (!prop->findAttribute("key", key))
						continue;
					if (!prop->findAttribute("value", value))
						continue;

					controllerInfo->mProperty[key] = value;
					if (item)
					{
						item->setProperty(key, value);
					}
				}
				if (item)
				{
					MyGUI::ControllerManager::getInstance().addItem(container->widget, item);
				}

				container->mController.push_back(controllerInfo);
			}
		};
	}

	bool EditorWidgets::tryToApplyProperty(MyGUI::Widget* _widget, const std::string& _key, const std::string& _value, bool _testMode)
	{
		WidgetContainer* container = EditorWidgets::getInstance().find(_widget);
		for (MyGUI::VectorStringPairs::iterator item = container->mUserString.begin(); item != container->mUserString.end(); ++item)
		{
			if ((*item).first == "LE_TargetWidgetType")
				return true;
		}

		try
		{
			if (_key == "Image_Texture")
			{
				if (!MyGUI::DataManager::getInstance().isDataExist(_value))
				{
					GroupMessage::getInstance().addMessage("No such " + _key + ": '" + _value + "'. This value will be saved.", MyGUI::LogLevel::Warning);
					return true;
				}
			}

			if (_testMode || std::find(mIgnoreParameters.begin(), mIgnoreParameters.end(), _key) == mIgnoreParameters.end())
			{
				_widget->setProperty(_key, _value);
			}
			//FIXME ����� ��� �� �����, ��� ���� ������ ��� �������� ������� ������� ��-�� ���������� ������� �����������
			//Ogre::Root::getSingleton().renderOneFrame();
		}
		catch (...)
		{
			GroupMessage::getInstance().addMessage("Can't apply '" + _key + "'property.", MyGUI::LogLevel::Error);
			return false;
		}
		return true;
	}

	void EditorWidgets::serialiseWidget(WidgetContainer* _container, MyGUI::xml::ElementPtr _node, bool _compatibility)
	{
		MyGUI::xml::ElementPtr node = _node->createChild("Widget");

		node->addAttribute("type", _container->type);
		node->addAttribute("skin", _container->skin);

		if (!_container->relative_mode)
			node->addAttribute("position", _container->position());
		else
			node->addAttribute("position_real", _container->position(false));

		if ("" != _container->align)
			node->addAttribute("align", _container->align);

		if ("" != _container->style)
			node->addAttribute("style", _container->style);

		if ("" != _container->getLayerName())
			node->addAttribute("layer", _container->getLayerName());

		if ("" != _container->name)
			node->addAttribute("name", _container->name);

		for (MyGUI::VectorStringPairs::iterator iter = _container->mProperty.begin(); iter != _container->mProperty.end(); ++iter)
			BackwardCompatibilityManager::getInstance().serialiseProperty(node, _container->type, *iter, _compatibility);

		for (MyGUI::VectorStringPairs::iterator iter = _container->mUserString.begin(); iter != _container->mUserString.end(); ++iter)
		{
			MyGUI::xml::ElementPtr nodeProp = node->createChild("UserString");
			nodeProp->addAttribute("key", iter->first);
			nodeProp->addAttribute("value", iter->second);
		}

		for (std::vector<ControllerInfo*>::iterator iter = _container->mController.begin(); iter != _container->mController.end(); ++iter)
		{
			MyGUI::xml::ElementPtr nodeController = node->createChild("Controller");
			nodeController->addAttribute("type", (*iter)->mType);
			for (MyGUI::MapString::iterator iterProp = (*iter)->mProperty.begin(); iterProp != (*iter)->mProperty.end(); ++iterProp)
			{
				MyGUI::xml::ElementPtr nodeProp = nodeController->createChild("Property");
				nodeProp->addAttribute("key", iterProp->first);
				nodeProp->addAttribute("value", iterProp->second);
			}
		}

		for (std::vector<WidgetContainer*>::iterator iter = _container->childContainers.begin(); iter != _container->childContainers.end(); ++iter)
		{
			serialiseWidget(*iter, node, _compatibility);
		}
	}

	void EditorWidgets::loadIgnoreParameters(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator parameter = _node->getElementEnumerator();
		while (parameter.next("Parameter"))
		{
			std::string name = parameter->findAttribute("key");
			mIgnoreParameters.push_back(name);
		}
	}

	void EditorWidgets::notifyFrameStarted(float _time)
	{
		if (mWidgetsChanged)
		{
			mWidgetsChanged = false;
			eventChangeWidgets();
		}
	}

	void EditorWidgets::invalidateWidgets()
	{
		mWidgetsChanged = true;
	}

	void EditorWidgets::loadSector(MyGUI::xml::ElementPtr _sectorNode)
	{
		SettingsSector* sector = new SettingsSector();
		sector->deserialization(_sectorNode, MyGUI::Version());

		mSettings.push_back(sector);
	}

	void EditorWidgets::saveSectors(MyGUI::xml::ElementPtr _rootNode)
	{
		for (VectorSettingsSector::iterator item = mSettings.begin(); item != mSettings.end(); ++item)
			(*item)->serialization(_rootNode, MyGUI::Version());
	}

	void EditorWidgets::destroyAllSectors()
	{
		for (VectorSettingsSector::iterator item = mSettings.begin(); item != mSettings.end(); ++item)
			delete (*item);
		mSettings.clear();
	}

	SettingsSector* EditorWidgets::getSector(const MyGUI::UString& _sectorName)
	{
		for (VectorSettingsSector::iterator item = mSettings.begin(); item != mSettings.end(); ++item)
		{
			if ((*item)->getName() == _sectorName)
				return (*item);
		}

		SettingsSector* sector = new SettingsSector();
		sector->setName(_sectorName);

		mSettings.push_back(sector);
		return sector;
	}

	EnumeratorWidgetContainer EditorWidgets::getWidgets()
	{
		return EnumeratorWidgetContainer(mWidgets);
	}

	std::string EditorWidgets::getSkinReplace(const std::string& _skinName)
	{
		MapString::iterator item = mSkinReplaces.find(_skinName);
		if (item != mSkinReplaces.end())
			return (*item).second;
		return _skinName;
	}

	void EditorWidgets::loadSkinReplace(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("Skin"))
			mSkinReplaces[node->findAttribute("key")] = node->getContent();
	}

	bool EditorWidgets::isSkinExist(const std::string& _skinName)
	{
		return _skinName == "Default" ||
			MyGUI::SkinManager::getInstance().isExist(_skinName) ||
			(MyGUI::LayoutManager::getInstance().isExist(_skinName) && checkTemplate(_skinName));
	}

	bool EditorWidgets::checkTemplate(const std::string& _skinName)
	{
		MyGUI::ResourceLayout* templateInfo = MyGUI::LayoutManager::getInstance().getByName(_skinName, false);
		if (templateInfo != nullptr)
		{
			const MyGUI::VectorWidgetInfo& data = templateInfo->getLayoutData();
			for (MyGUI::VectorWidgetInfo::const_iterator container = data.begin(); container != data.end(); ++container)
			{
				if (container->name == "Root")
					return true;
			}
		}

		return false;
	}

	const MyGUI::UString& EditorWidgets::getCurrentFileName() const
	{
		return mCurrentFileName;
	}

	const MyGUI::UString& EditorWidgets::getCurrentItemName() const
	{
		return mCurrentItemName;
	}

	void EditorWidgets::_unlinkWidget(MyGUI::Widget* _widget)
	{
		WidgetContainer* container = find(_widget);
		if (container != nullptr)
		{
			bool result = unbind(container);
			mWidgetsChanged = true;

			if (result)
				WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
		}
	}

	void EditorWidgets::loadWidgetsFromXmlNode(MyGUI::xml::ElementPtr _root, bool _testMode)
	{
		// ����� ����� � ��������
		MyGUI::xml::ElementEnumerator element = _root->getElementEnumerator();
		while (element.next())
		{
			if (element->getName() == "Widget")
				parseWidget(element, nullptr, _testMode);
			else
				loadSector(element.current());
		}
	}

	void EditorWidgets::saveWidgetsToXmlNode(MyGUI::xml::ElementPtr _root, bool _compatibility)
	{
		_root->addAttribute("version", BackwardCompatibilityManager::getInstancePtr()->getCurrentVersion());

		for (std::vector<WidgetContainer*>::iterator iter = mWidgets.begin(); iter != mWidgets.end(); ++iter)
		{
			// � ������ ������ �����
			if (nullptr == (*iter)->widget->getParent())
				serialiseWidget(*iter, _root, _compatibility);
		}

		saveSectors(_root);
	}

	void EditorWidgets::onSetWidgetCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _value, const std::string& _owner)
	{
		eventChangeWidgetCoord(_widget, _value, _owner);
	}

} // namespace tools
