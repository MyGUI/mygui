#include "Precompiled.h"
#include "EditorWidgets.h"
#include "Common.h"
#include "WidgetTypes.h"
#include "GroupMessage.h"
#include "BackwardCompatibilityManager.h"
#include "WidgetSelectorManager.h"
#include "SettingsManager.h"

namespace tools
{
	MYGUI_SINGLETON_DEFINITION(EditorWidgets);

	const std::string_view LogSection = "LayoutEditor";
	const std::string_view CodeGeneratorSettingsNodeName = "CodeGeneratorSettings";

	EditorWidgets::EditorWidgets() :
		mSingletonHolder(this)
	{
	}

	void EditorWidgets::initialise()
	{
		mWidgetsChanged = true;

		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("IgnoreParameters") =
			MyGUI::newDelegate(this, &EditorWidgets::loadIgnoreParameters);
		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("SkinReplace") =
			MyGUI::newDelegate(this, &EditorWidgets::loadSkinReplace);

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &EditorWidgets::notifyFrameStarted);
		MyGUI::WidgetManager::getInstance().registerUnlinker(this);
	}

	void EditorWidgets::shutdown()
	{
		MyGUI::WidgetManager::getInstance().unregisterUnlinker(this);
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &EditorWidgets::notifyFrameStarted);

		destroyAllWidgets();

		mCodeGeneratorSettings.clear();
	}

	void EditorWidgets::destroyAllWidgets()
	{
		for (auto& widget : mWidgets)
			delete widget;
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
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator element = root->getElementEnumerator();
			while (element.next("Resource"))
			{
				if (element->findAttribute("type") == "ResourceLayout")
				{
					if (_index == 0)
					{
						mCurrentItemName.assign(element->findAttribute("name"));

						loadWidgetsFromXmlNode(element.current());

						break;
					}

					_index--;
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
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator element = root->getElementEnumerator();
			while (element.next("Resource"))
			{
				if (element->findAttribute("type") == "ResourceLayout")
				{
					if (_index == 0)
					{
						mCurrentItemName.assign(element->findAttribute("name"));

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

					_index--;
				}
			}
			return false;
		}

		return false;
	}

	void EditorWidgets::loadxmlDocument(MyGUI::xml::Document* doc, bool _testMode)
	{
		MyGUI::xml::ElementPtr root = doc->getRoot();

		std::string_view type = root->findAttribute("type");
		if (type == "Layout")
		{
			loadWidgetsFromXmlNode(root, _testMode);
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
		if (nullptr == _container->getWidget()->getParent())
		{
			mWidgets.push_back(_container);
		}
		else
		{
			MyGUI::Widget* parent = _container->getWidget()->getParent();
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
		if (nullptr != _container)
		{
			std::vector<WidgetContainer*>::reverse_iterator iter;
			while (!_container->childContainers.empty())
			{
				iter = _container->childContainers.rbegin();
				remove(*iter);
			}

			if (nullptr == _container->getWidget()->getParent())
			{
				mWidgets.erase(std::find(mWidgets.begin(), mWidgets.end(), _container));
			}
			else
			{
				MyGUI::Widget* parent = _container->getWidget()->getParent();
				WidgetContainer* containerParent = find(parent);
				while (nullptr == containerParent)
				{
					parent = parent->getParent();
					if (parent == nullptr)
					{
						// такого не должно быть, или утечка памяти,
						// так как удалять нельзя им пользуется код вызывающий данную функцию
						return;
					}
					containerParent = find(parent);
				}

				containerParent->childContainers.erase(std::find(
					containerParent->childContainers.begin(),
					containerParent->childContainers.end(),
					_container));
			}

			MyGUI::Gui::getInstance().destroyWidget(_container->getWidget());

			delete _container;
		}

		mWidgetsChanged = true;
	}

	bool EditorWidgets::unbind(WidgetContainer* _container)
	{
		bool result = false;

		if (nullptr != _container)
		{
			std::vector<WidgetContainer*>::reverse_iterator iter;
			while (!_container->childContainers.empty())
			{
				iter = _container->childContainers.rbegin();
				if (unbind(*iter))
					result = true;
			}

			if (nullptr == _container->getWidget()->getParent())
			{
				mWidgets.erase(std::find(mWidgets.begin(), mWidgets.end(), _container));
			}
			else
			{
				MyGUI::Widget* parent = _container->getWidget()->getParent();
				WidgetContainer* containerParent = find(parent);
				while (nullptr == containerParent)
				{
					parent = parent->getParent();
					if (parent == nullptr)
					{
						// такого не должно быть, или утечка памяти,
						// так как удалять нельзя им пользуется код вызывающий данную функцию
						return result;
					}
					containerParent = find(parent);
				}

				containerParent->childContainers.erase(std::find(
					containerParent->childContainers.begin(),
					containerParent->childContainers.end(),
					_container));
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
			remove(mWidgets[mWidgets.size() - 1]);
		}

		mCodeGeneratorSettings.clear();
	}

	WidgetContainer* EditorWidgets::find(MyGUI::Widget* _widget)
	{
		return _find(_widget, {}, mWidgets);
	}

	WidgetContainer* EditorWidgets::find(std::string_view _name)
	{
		return _find(nullptr, _name, mWidgets);
	}

	WidgetContainer* EditorWidgets::_find(
		MyGUI::Widget* _widget,
		std::string_view _name,
		std::vector<WidgetContainer*> _widgets)
	{
		for (auto& iter : _widgets)
		{
			if ((iter->getWidget() == _widget) || (!_name.empty() && iter->getName() == _name))
			{
				return iter;
			}
			WidgetContainer* retContainer = _find(_widget, _name, iter->childContainers);
			if (retContainer)
				return retContainer;
		}
		return nullptr;
	}

	void EditorWidgets::parseWidget(MyGUI::xml::ElementEnumerator& _widget, MyGUI::Widget* _parent, bool _testMode)
	{
		WidgetContainer* container = new WidgetContainer();
		// парсим атрибуты виджета
		MyGUI::IntCoord coord;
		MyGUI::Align align = MyGUI::Align::Default;
		MyGUI::WidgetStyle widgetStyle = MyGUI::WidgetStyle::Child;
		std::string position;

		container->setName(_widget->findAttribute("name"));
		container->setType(_widget->findAttribute("type"));
		container->setSkin(_widget->findAttribute("skin"));
		container->setLayerName(_widget->findAttribute("layer"));
		std::string tmp;
		if (_widget->findAttribute("style", tmp))
		{
			container->setStyle(tmp);
			widgetStyle = MyGUI::WidgetStyle::parse(tmp);
		}
		if (_widget->findAttribute("align", tmp))
		{
			container->setAlign(tmp);
			align = MyGUI::Align::parse(tmp);
		}
		if (_widget->findAttribute("position", position))
			coord = MyGUI::IntCoord::parse(position);
		if (_widget->findAttribute("position_real", position))
		{
			container->setRelativeMode(true);
			MyGUI::IntSize textureSize =
				SettingsManager::getInstance().getValue<MyGUI::IntSize>("Settings/WorkspaceTextureSize");
			MyGUI::IntSize size = _testMode ? MyGUI::RenderManager::getInstance().getViewSize() : textureSize;
			coord = MyGUI::CoordConverter::convertFromRelative(
				MyGUI::DoubleCoord::parse(position),
				_parent == nullptr ? size : _parent->getClientCoord().size());
		}

		// проверяем скин на присутствие
		std::string_view skin = container->getSkin();
		bool exist = isSkinExist(container->getSkin());
		if (!exist && !container->getSkin().empty())
		{
			skin = WidgetTypes::getInstance().findWidgetStyle(container->getType())->default_skin;

			std::string skin_string;
			if (skin.empty())
				skin_string = "empty skin";
			else
				skin_string = "'" + std::string{skin} + "'";

			// FIXME : not translated string
			std::string mess = MyGUI::utility::toString(
				"'",
				container->getSkin(),
				"' skin not found , temporary changed to ",
				skin_string);
			GroupMessage::getInstance().addMessage(mess, MyGUI::LogLevel::Error);
		}

		if (!_testMode)
			skin = getSkinReplace(skin);

		std::string_view layer = DEFAULT_EDITOR_LAYER;
		if (_testMode)
		{
			// use widget's layer if possible
			if (MyGUI::LayerManager::getInstance().isExist(container->getLayerName()))
				layer = container->getLayerName();
			else
				layer = DEFAULT_TEST_MODE_LAYER;
		}
		std::string_view widgetType =
			MyGUI::FactoryManager::getInstance().isFactoryExist("Widget", container->getType())
			? container->getType()
			: MyGUI::Widget::getClassTypeName();

		if (nullptr == _parent)
		{
			container->setWidget(MyGUI::Gui::getInstance().createWidgetT(widgetType, skin, coord, align, layer));
		}
		else
		{
			container->setWidget(_parent->createWidgetT(widgetStyle, widgetType, skin, coord, align, layer));
		}

		add(container);

		// берем детей и крутимся
		MyGUI::xml::ElementEnumerator widget = _widget->getElementEnumerator();
		while (widget.next())
		{
			std::string key;
			std::string value;
			std::string type;

			if (widget->getName() == "Widget")
			{
				parseWidget(widget, container->getWidget(), _testMode);
			}
			else if (widget->getName() == "Property")
			{
				// парсим атрибуты
				if (!widget->findAttribute("key", key))
					continue;
				if (!widget->findAttribute("value", value))
					continue;

				// конвертим проперти в текущий вариант версии
				key = MyGUI::BackwardCompatibility::getPropertyRename(key);
				size_t indexSeparator = key.find('_');
				if (indexSeparator != std::string::npos)
					key = key.substr(indexSeparator + 1);

				// и пытаемся парсить свойство
				if (!tryToApplyProperty(container->getWidget(), key, value, _testMode))
					continue;

				container->setProperty(key, value, false);
			}
			else if (widget->getName() == "UserString")
			{
				// парсим атрибуты
				if (!widget->findAttribute("key", key))
					continue;
				if (!widget->findAttribute("value", value))
					continue;
				//container->mUserString.insert(MyGUI::PairString(key, value));
				container->setUserData(key, value);
			}
			else if (widget->getName() == "Controller")
			{
				// парсим атрибуты
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
					MyGUI::ControllerManager::getInstance().addItem(container->getWidget(), item);
				}

				container->mController.push_back(controllerInfo);
			}
		}
	}

	bool EditorWidgets::tryToApplyProperty(
		MyGUI::Widget* _widget,
		std::string_view _key,
		std::string_view _value,
		bool _testMode)
	{
		WidgetContainer* container = EditorWidgets::getInstance().find(_widget);
		if (container->existUserData("LE_TargetWidgetType"))
			return true;

		try
		{
			if (_key == "Image_Texture")
			{
				std::string value{_value};
				if (!MyGUI::DataManager::getInstance().isDataExist(value))
				{
					GroupMessage::getInstance().addMessage(
						"No such " + std::string{_key} + ": '" + value + "'. This value will be saved.",
						MyGUI::LogLevel::Warning);
					return true;
				}
			}

			if (_testMode ||
				std::find(mIgnoreParameters.begin(), mIgnoreParameters.end(), _key) == mIgnoreParameters.end())
			{
				_widget->setProperty(_key, _value);
			}
		}
		catch (...)
		{
			GroupMessage::getInstance().addMessage(
				"Can't apply '" + std::string{_key} + "'property.",
				MyGUI::LogLevel::Error);
			return false;
		}
		return true;
	}

	void EditorWidgets::serialiseWidget(WidgetContainer* _container, MyGUI::xml::ElementPtr _node, bool _compatibility)
	{
		MyGUI::xml::ElementPtr node = _node->createChild("Widget");

		node->addAttribute("type", _container->getType());
		node->addAttribute("skin", _container->getSkin());

		if (!_container->getRelativeMode())
			node->addAttribute("position", _container->position());
		else
			node->addAttribute("position_real", _container->position(false));

		if (!_container->getAlign().empty())
			node->addAttribute("align", _container->getAlign());

		if (!_container->getStyle().empty())
			node->addAttribute("style", _container->getStyle());

		if (!_container->getLayerName().empty())
			node->addAttribute("layer", _container->getLayerName());

		if (!_container->getName().empty())
			node->addAttribute("name", _container->getName());

		WidgetContainer::PropertyEnumerator propertyItem = _container->getPropertyEnumerator();
		while (propertyItem.next())
		{
			BackwardCompatibilityManager::getInstance()
				.serialiseProperty(node, _container->getType(), propertyItem.current(), _compatibility);
		}

		WidgetContainer::UserDataEnumerator userData = _container->getUserDataEnumerator();
		while (userData.next())
		{
			MyGUI::xml::ElementPtr nodeProp = node->createChild("UserString");
			nodeProp->addAttribute("key", userData.current().first);
			nodeProp->addAttribute("value", userData.current().second);
		}

		for (auto& iter : _container->mController)
		{
			MyGUI::xml::ElementPtr nodeController = node->createChild("Controller");
			nodeController->addAttribute("type", iter->mType);
			for (auto& iterProp : iter->mProperty)
			{
				MyGUI::xml::ElementPtr nodeProp = nodeController->createChild("Property");
				nodeProp->addAttribute("key", iterProp.first);
				nodeProp->addAttribute("value", iterProp.second);
			}
		}

		for (auto& childContainer : _container->childContainers)
		{
			serialiseWidget(childContainer, node, _compatibility);
		}
	}

	void EditorWidgets::loadIgnoreParameters(
		MyGUI::xml::ElementPtr _node,
		std::string_view /*_file*/,
		MyGUI::Version /*_version*/)
	{
		MyGUI::xml::ElementEnumerator parameter = _node->getElementEnumerator();
		while (parameter.next("Parameter"))
		{
			std::string_view name = parameter->findAttribute("key");
			mIgnoreParameters.emplace_back(name);
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

	EnumeratorWidgetContainer EditorWidgets::getWidgets()
	{
		return EnumeratorWidgetContainer(mWidgets);
	}

	std::string_view EditorWidgets::getSkinReplace(std::string_view _skinName)
	{
		MyGUI::MapString::iterator item = mSkinReplaces.find(_skinName);
		if (item != mSkinReplaces.end())
			return (*item).second;
		return _skinName;
	}

	void EditorWidgets::loadSkinReplace(
		MyGUI::xml::ElementPtr _node,
		std::string_view /*_file*/,
		MyGUI::Version /*_version*/)
	{
		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("Skin"))
		{
			MyGUI::mapSet(mSkinReplaces, node->findAttribute("key"), node->getContent());
		}
	}

	bool EditorWidgets::isSkinExist(std::string_view _skinName)
	{
		return _skinName == "Default" || MyGUI::SkinManager::getInstance().isExist(_skinName) ||
			(MyGUI::LayoutManager::getInstance().isExist(_skinName) && checkTemplate(_skinName));
	}

	bool EditorWidgets::checkTemplate(std::string_view _skinName)
	{
		MyGUI::ResourceLayout* templateInfo = MyGUI::LayoutManager::getInstance().getByName(_skinName, false);
		if (templateInfo != nullptr)
		{
			const MyGUI::VectorWidgetInfo& data = templateInfo->getLayoutData();
			for (const auto& container : data)
			{
				if (container.name == "Root")
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

	MyGUI::MapString& EditorWidgets::getCodeGeneratorSettings()
	{
		return mCodeGeneratorSettings;
	}

	void EditorWidgets::loadWidgetsFromXmlNode(MyGUI::xml::ElementPtr _root, bool _testMode)
	{
		// берем детей и крутимся
		MyGUI::xml::ElementEnumerator element = _root->getElementEnumerator();
		while (element.next())
		{
			if (element->getName() == "Widget")
				parseWidget(element, nullptr, _testMode);
			else if (element->getName() == CodeGeneratorSettingsNodeName)
				loadCodeGeneratorSettings(element.current());
		}
	}

	void EditorWidgets::saveWidgetsToXmlNode(MyGUI::xml::ElementPtr _root, bool _compatibility)
	{
		_root->addAttribute("version", BackwardCompatibilityManager::getInstancePtr()->getCurrentVersion());

		for (auto& widget : mWidgets)
		{
			// в корень только сирот
			if (nullptr == widget->getWidget()->getParent())
				serialiseWidget(widget, _root, _compatibility);
		}

		saveCodeGeneratorSettings(_root);
	}

	void EditorWidgets::loadCodeGeneratorSettings(MyGUI::xml::ElementPtr _sectorNode)
	{
		MyGUI::xml::ElementEnumerator widget = _sectorNode->getElementEnumerator();
		while (widget.next())
		{
			if (widget->getName() == "Property")
			{
				std::string key;
				std::string value;

				// парсим атрибуты
				if (!widget->findAttribute("key", key))
					continue;
				if (!widget->findAttribute("value", value))
					continue;

				mCodeGeneratorSettings[key] = value;
			}
		}
	}

	void EditorWidgets::saveCodeGeneratorSettings(MyGUI::xml::ElementPtr _rootNode)
	{
		MyGUI::xml::ElementPtr node = _rootNode->createChild(CodeGeneratorSettingsNodeName);

		for (MyGUI::MapString::const_iterator iter = mCodeGeneratorSettings.begin();
			 iter != mCodeGeneratorSettings.end();
			 ++iter)
		{
			MyGUI::xml::ElementPtr nodeProp = node->createChild("Property");
			nodeProp->addAttribute("key", iter->first);
			nodeProp->addAttribute("value", iter->second);
		}
	}

	void EditorWidgets::onSetWidgetCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _value, std::string_view _owner)
		const
	{
		eventChangeWidgetCoord(_widget, _value, _owner);
	}

}
