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

		pugi::xml_node root = doc.getRoot();
		if (!root || std::string_view(root.name()) != "MyGUI")
		{
			MYGUI_LOGGING(LogSection, Error, getClassTypeName() << " : '" << _fileName << "', tag 'MyGUI' not found");
			return false;
		}

		if (std::string_view(root.attribute("type").value()) == "Layout")
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

		pugi::xml_node root = doc.getRoot();
		if (!root || std::string_view(root.name()) != "MyGUI")
		{
			MYGUI_LOGGING(LogSection, Error, getClassTypeName() << " : '" << _fileName << "', tag 'MyGUI' not found");
			return false;
		}

		if (std::string_view(root.attribute("type").value()) == "Resource")
		{
			// берем детей и крутимся
			for (auto element : root.children("Resource"))
			{
				if (std::string_view(element.attribute("type").value()) == "ResourceLayout")
				{
					if (_index == 0)
					{
						mCurrentItemName.assign(std::string_view(element.attribute("name").value()));

						loadWidgetsFromXmlNode(element);

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
		pugi::xml_node root = doc.createRoot("MyGUI");
		root.append_attribute("type") = "Layout";

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

		pugi::xml_node root = doc.getRoot();
		if (!root || std::string_view(root.name()) != "MyGUI")
		{
			MYGUI_LOGGING(LogSection, Error, getClassTypeName() << " : '" << _fileName << "', tag 'MyGUI' not found");
			return false;
		}

		if (std::string_view(root.attribute("type").value()) == "Resource")
		{
			// берем детей и крутимся
			for (auto element : root.children("Resource"))
			{
				if (std::string_view(element.attribute("type").value()) == "ResourceLayout")
				{
					if (_index == 0)
					{
						mCurrentItemName.assign(std::string_view(element.attribute("name").value()));

						element.remove_attributes();
						element.append_attribute("type") = "ResourceLayout";
						element.append_attribute("name") = mCurrentItemName.asUTF8_c_str();

						saveWidgetsToXmlNode(element, true);

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
		pugi::xml_node root = doc->getRoot();

		std::string_view type = root.attribute("type").value();
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
		pugi::xml_node root = doc->createRoot("MyGUI");
		root.append_attribute("type") = "Layout";

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

				containerParent->childContainers.erase(
					std::find(
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

				containerParent->childContainers.erase(
					std::find(
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

	void EditorWidgets::parseWidget(pugi::xml_node _widget, MyGUI::Widget* _parent, bool _testMode)
	{
		WidgetContainer* container = new WidgetContainer();
		// парсим атрибуты виджета
		MyGUI::IntCoord coord;
		MyGUI::Align align = MyGUI::Align::Default;
		MyGUI::WidgetStyle widgetStyle = MyGUI::WidgetStyle::Child;
		std::string position;

		container->setName(_widget.attribute("name").value());
		container->setType(_widget.attribute("type").value());
		container->setSkin(_widget.attribute("skin").value());
		container->setLayerName(_widget.attribute("layer").value());
		std::string tmp;
		if (auto attr = _widget.attribute("style"))
		{
			tmp = attr.value();
			container->setStyle(tmp);
			widgetStyle = MyGUI::WidgetStyle::parse(tmp);
		}
		if (auto attr = _widget.attribute("align"))
		{
			tmp = attr.value();
			container->setAlign(tmp);
			align = MyGUI::Align::parse(tmp);
		}
		if (auto attr = _widget.attribute("position"))
		{
			position = attr.value();
			coord = MyGUI::IntCoord::parse(position);
		}
		if (auto attr = _widget.attribute("position_real"))
		{
			position = attr.value();
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
		for (auto widget : _widget.children())
		{
			std::string key;
			std::string value;
			std::string type;

			if (std::string_view(widget.name()) == "Widget")
			{
				parseWidget(widget, container->getWidget(), _testMode);
			}
			else if (std::string_view(widget.name()) == "Property")
			{
				// парсим атрибуты
				if (auto attr = widget.attribute("key"))
					key = attr.value();
				else
					continue;
				if (auto attr = widget.attribute("value"))
					value = attr.value();
				else
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
			else if (std::string_view(widget.name()) == "UserString")
			{
				// парсим атрибуты
				if (auto attr = widget.attribute("key"))
					key = attr.value();
				else
					continue;
				if (auto attr = widget.attribute("value"))
					value = attr.value();
				else
					continue;
				//container->mUserString.insert(MyGUI::PairString(key, value));
				container->setUserData(key, value);
			}
			else if (std::string_view(widget.name()) == "Controller")
			{
				// парсим атрибуты
				if (auto attr = widget.attribute("type"))
					type = attr.value();
				else
					continue;
				ControllerInfo* controllerInfo = new ControllerInfo();
				controllerInfo->mType = type;

				MyGUI::ControllerItem* item = nullptr;
				if (_testMode)
				{
					item = MyGUI::ControllerManager::getInstance().createItem(type);
				}

				for (auto prop : widget.children("Property"))
				{
					if (auto attr = prop.attribute("key"))
						key = attr.value();
					else
						continue;
					if (auto attr = prop.attribute("value"))
						value = attr.value();
					else
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

	void EditorWidgets::serialiseWidget(WidgetContainer* _container, pugi::xml_node _node, bool _compatibility)
	{
		pugi::xml_node node = _node.append_child("Widget");

		node.append_attribute("type") = _container->getType().data();
		node.append_attribute("skin") = _container->getSkin().data();

		if (!_container->getRelativeMode())
			node.append_attribute("position") = _container->position().c_str();
		else
			node.append_attribute("position_real") = _container->position(false).c_str();

		if (!_container->getAlign().empty())
			node.append_attribute("align") = _container->getAlign().data();

		if (!_container->getStyle().empty())
			node.append_attribute("style") = _container->getStyle().data();

		if (!_container->getLayerName().empty())
			node.append_attribute("layer") = _container->getLayerName().data();

		if (!_container->getName().empty())
			node.append_attribute("name") = _container->getName().data();

		WidgetContainer::PropertyEnumerator propertyItem = _container->getPropertyEnumerator();
		while (propertyItem.next())
		{
			BackwardCompatibilityManager::getInstance()
				.serialiseProperty(node, _container->getType(), propertyItem.current(), _compatibility);
		}

		WidgetContainer::UserDataEnumerator userData = _container->getUserDataEnumerator();
		while (userData.next())
		{
			pugi::xml_node nodeProp = node.append_child("UserString");
			nodeProp.append_attribute("key") = userData.current().first.data();
			nodeProp.append_attribute("value") = userData.current().second.data();
		}

		for (auto& iter : _container->mController)
		{
			pugi::xml_node nodeController = node.append_child("Controller");
			nodeController.append_attribute("type") = iter->mType.data();
			for (auto& iterProp : iter->mProperty)
			{
				pugi::xml_node nodeProp = nodeController.append_child("Property");
				nodeProp.append_attribute("key") = iterProp.first.data();
				nodeProp.append_attribute("value") = iterProp.second.data();
			}
		}

		for (auto& childContainer : _container->childContainers)
		{
			serialiseWidget(childContainer, node, _compatibility);
		}
	}

	void EditorWidgets::loadIgnoreParameters(
		pugi::xml_node _node,
		std::string_view /*_file*/,
		MyGUI::Version /*_version*/)
	{
		for (auto parameter : _node.children("Parameter"))
		{
			std::string_view name = parameter.attribute("key").value();
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

	void EditorWidgets::loadSkinReplace(pugi::xml_node _node, std::string_view /*_file*/, MyGUI::Version /*_version*/)
	{
		for (auto node : _node.children("Skin"))
		{
			MyGUI::mapSet(mSkinReplaces, node.attribute("key").value(), node.child_value());
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

	void EditorWidgets::loadWidgetsFromXmlNode(pugi::xml_node _root, bool _testMode)
	{
		// берем детей и крутимся
		for (auto element : _root.children())
		{
			if (std::string_view(element.name()) == "Widget")
				parseWidget(element, nullptr, _testMode);
			else if (std::string_view(element.name()) == CodeGeneratorSettingsNodeName)
				loadCodeGeneratorSettings(element);
		}
	}

	void EditorWidgets::saveWidgetsToXmlNode(pugi::xml_node _root, bool _compatibility)
	{
		_root.append_attribute("version") = BackwardCompatibilityManager::getInstancePtr()->getCurrentVersion().c_str();

		for (auto& widget : mWidgets)
		{
			// в корень только сирот
			if (nullptr == widget->getWidget()->getParent())
				serialiseWidget(widget, _root, _compatibility);
		}

		saveCodeGeneratorSettings(_root);
	}

	void EditorWidgets::loadCodeGeneratorSettings(pugi::xml_node _sectorNode)
	{
		for (auto widget : _sectorNode.children())
		{
			if (std::string_view(widget.name()) == "Property")
			{
				std::string key;
				std::string value;

				// парсим атрибуты
				if (auto attr = widget.attribute("key"))
					key = attr.value();
				else
					continue;
				if (auto attr = widget.attribute("value"))
					value = attr.value();
				else
					continue;

				mCodeGeneratorSettings[key] = value;
			}
		}
	}

	void EditorWidgets::saveCodeGeneratorSettings(pugi::xml_node _rootNode)
	{
		pugi::xml_node node = _rootNode.append_child(CodeGeneratorSettingsNodeName.data());

		for (MyGUI::MapString::const_iterator iter = mCodeGeneratorSettings.begin();
			 iter != mCodeGeneratorSettings.end();
			 ++iter)
		{
			pugi::xml_node nodeProp = node.append_child("Property");
			nodeProp.append_attribute("key") = iter->first.data();
			nodeProp.append_attribute("value") = iter->second.data();
		}
	}

	void EditorWidgets::onSetWidgetCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _value, std::string_view _owner)
		const
	{
		eventChangeWidgetCoord(_widget, _value, _owner);
	}

}
