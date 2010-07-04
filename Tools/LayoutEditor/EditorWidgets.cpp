#include "precompiled.h"
#include "Common.h"
#include "EditorWidgets.h"
//#include "BasisManager.h"
#include "WidgetTypes.h"
#include "GroupMessage.h"
#include "CodeGenerator.h"

const std::string LogSection = "LayoutEditor";

template <> const char* MyGUI::Singleton<EditorWidgets>::mClassTypeName("EditorWidgets");

EditorWidgets::EditorWidgets() :
	global_counter(0),
	widgets_changed(false),
	mCodeGenerator(nullptr)
{
}
EditorWidgets::~EditorWidgets()
{
}

void MapSet(MyGUI::VectorStringPairs & _map, const std::string &_key, const std::string &_value)
{
	bool find = false;
	for (MyGUI::VectorStringPairs::iterator iter=_map.begin(); iter!=_map.end(); ++iter)
	{
		if (iter->first == _key)
		{
			iter->second = _value;
			find = true;
		}
	}
	if (!find)
	{
		_map.push_back(MyGUI::PairString(_key, _value));
	}
}

MyGUI::VectorStringPairs::iterator MapFind(MyGUI::VectorStringPairs & _map, const std::string &_key)
{
	MyGUI::VectorStringPairs::iterator iter = _map.begin();
	for (; iter!=_map.end(); ++iter)
	{
		if (iter->first == _key) break;
	}
	return iter;
}

void MapErase(MyGUI::VectorStringPairs & _map, const std::string &_key)
{
	for (MyGUI::VectorStringPairs::iterator iter = _map.begin(); iter != _map.end(); ++iter)
	{
		if (iter->first == _key)
		{
			_map.erase(iter);
			return;
		}
	}
}

MyGUI::IntCoord convertCoordToParentCoord(const MyGUI::IntCoord& _coord, MyGUI::Widget* widget)
{
	MyGUI::IntCoord coord = _coord;
	MyGUI::Widget* parent = widget->getParent();
	while (nullptr != parent)
	{
		coord = coord - parent->getPosition();
		// а может у нас и дедушка есть? а может и прадед...
		parent = parent->getParent();
	}
	return coord;
}

void EditorWidgets::initialise()
{
	global_counter = 0;
	widgets_changed = true;

	MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("IgnoreParameters") = MyGUI::newDelegate(this, &EditorWidgets::loadIgnoreParameters);
}

void EditorWidgets::shutdown()
{
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter) delete *iter;
	widgets.clear();
}

bool EditorWidgets::load(const MyGUI::UString& _fileName)
{
	std::string _instance = "Editor";

	MyGUI::xml::Document doc;
	if (!doc.open(_fileName))
	{
		MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return false;
	}

	MyGUI::xml::ElementPtr root = doc.getRoot();
	if ( (nullptr == root) || (root->getName() != "MyGUI") )
	{
		MYGUI_LOGGING(LogSection, Error, _instance << " : '" << _fileName << "', tag 'MyGUI' not found");
		return false;
	}

	std::string type;
	if (root->findAttribute("type", type))
	{
		if (type == "Layout")
		{
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator element = root->getElementEnumerator();
			while (element.next("Widget")) parseWidget(element, nullptr);
			element = root->getElementEnumerator();
			while (element.next("CodeGenaratorSettings")) mCodeGenerator->loadProperties(element);
		}
		else
		{
			return false;
		}
	}

	widgets_changed = true;
	return true;
}

bool EditorWidgets::save(const MyGUI::UString& _fileName)
{
	std::string _instance = "Editor";

	MyGUI::xml::Document doc;
	doc.createDeclaration();
	MyGUI::xml::ElementPtr root = doc.createRoot("MyGUI");
	root->addAttribute("type", "Layout");

	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		// в корень только сирот
		if (nullptr == (*iter)->widget->getParent()) serialiseWidget(*iter, root);
	}

	mCodeGenerator->saveProperties(root);

	if (!doc.save(_fileName))
	{
		MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return false;
	}

	return true;
}

void EditorWidgets::loadxmlDocument(MyGUI::xml::Document * doc, bool _test)
{
	MyGUI::xml::ElementPtr root = doc->getRoot();

	std::string type;
	if (root->findAttribute("type", type))
	{
		if (type == "Layout")
		{
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator widget = root->getElementEnumerator();
			while (widget.next("Widget")) parseWidget(widget, 0, _test);
		}
	}
	widgets_changed = true;
}

MyGUI::xml::Document * EditorWidgets::savexmlDocument()
{
	MyGUI::xml::Document * doc = new MyGUI::xml::Document();

	doc->createDeclaration();
	MyGUI::xml::ElementPtr root = doc->createRoot("MyGUI");
	root->addAttribute("type", "Layout");

	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		// в корень только сирот
		if (nullptr == (*iter)->widget->getParent()) serialiseWidget(*iter, root);
	}

	return doc;
}

void EditorWidgets::add(WidgetContainer * _container)
{
	if (nullptr == _container->widget->getParent())
	{
		if ("" == _container->layer) _container->layer = DEFAULT_LAYER;
		widgets.push_back(_container);
	}
	else
	{
		MyGUI::Widget* parent = _container->widget->getParent();
		WidgetContainer * containerParent = find(parent);
		while (NULL == containerParent)
		{
			parent = parent->getParent();
			if (parent == nullptr) return;
			containerParent = find(parent);
		}
		containerParent->childContainers.push_back(_container);
	}
	widgets_changed = true;
}

void EditorWidgets::remove(MyGUI::Widget* _widget)
{
	remove(find(_widget));
	widgets_changed = true;
}

void EditorWidgets::remove(WidgetContainer * _container)
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
			widgets.erase(std::find(widgets.begin(), widgets.end(), _container));
		}
		else
		{
			MyGUI::Widget* parent = _container->widget->getParent();
			WidgetContainer * containerParent = find(parent);
			while (NULL == containerParent)
			{
				parent = parent->getParent();
				if (parent == nullptr) return;
				containerParent = find(parent);
			}

			containerParent->childContainers.erase(std::find(containerParent->childContainers.begin(), containerParent->childContainers.end(), _container));
		}

		MyGUI::Gui::getInstance().destroyWidget(_container->widget);

		delete _container;
	}
	widgets_changed = true;
}

void EditorWidgets::clear()
{
	while (!widgets.empty())
	{
		remove(widgets[widgets.size()-1]);
	}
	global_counter = 0;
}

WidgetContainer * EditorWidgets::find(MyGUI::Widget* _widget)
{
	return _find(_widget, "", widgets);
}

WidgetContainer * EditorWidgets::find(const std::string& _name)
{
	return _find(NULL, _name, widgets);
}

WidgetContainer * EditorWidgets::_find(MyGUI::Widget* _widget, const std::string& _name, std::vector<WidgetContainer*> _widgets)
{
	for (std::vector<WidgetContainer*>::iterator iter = _widgets.begin(); iter != _widgets.end(); ++iter)
	{
		if (((*iter)->widget == _widget) || ((_name.empty() == false) && ((*iter)->name == _name)))
		{
			return *iter;
		}
		WidgetContainer * retContainer = _find(_widget, _name, (*iter)->childContainers);
		if (retContainer) return retContainer;
	}
	return nullptr;
}

void EditorWidgets::parseWidget(MyGUI::xml::ElementEnumerator & _widget, MyGUI::Widget* _parent, bool _test)
{
	WidgetContainer * container = new WidgetContainer();
	// парсим атрибуты виджета
	MyGUI::IntCoord coord;
	MyGUI::Align align = MyGUI::Align::Default;
	std::string position;
	//std::string layer;

	_widget->findAttribute("name", container->name);
	_widget->findAttribute("type", container->type);
	_widget->findAttribute("skin", container->skin);
	_widget->findAttribute("layer", container->layer);
	if (_widget->findAttribute("align", container->align)) align = MyGUI::Align::parse(container->align);
	if (_widget->findAttribute("position", position)) coord = MyGUI::IntCoord::parse(position);
	if (_widget->findAttribute("position_real", position))
	{
		container->relative_mode = true;
		//FIXME парент может быть и не кроппед
		coord = MyGUI::CoordConverter::convertFromRelative(MyGUI::FloatCoord::parse(position), _parent == nullptr ? MyGUI::RenderManager::getInstance().getViewSize() : _parent->getClientCoord().size());
	}

	// в гуе на 2 одноименных виджета ругается и падает, а у нас будет просто переименовывать
	if (!container->name.empty())
	{
		WidgetContainer * iter = find(container->name);
		if (nullptr != iter)
		{
			static long renameN=0;
			// FIXME : not translated string
			std::string mess = MyGUI::utility::toString("Widget with name '", container->name, "' already exist. Renamed to '", container->name, renameN, "'.");
			MYGUI_LOGGING(LogSection, Warning, mess);
			GroupMessage::getInstance().addMessage(mess, MyGUI::LogLevel::Warning);
			container->name = MyGUI::utility::toString(container->name, renameN++);
		}
	}

	std::string tmpname = container->name;
	if (tmpname.empty())
	{
		tmpname = MyGUI::utility::toString(container->type, global_counter);
		global_counter++;
	}

	//может и не стоит
	tmpname = "LayoutEditorWidget_" + tmpname;

	// проверяем скин на присутствие
	std::string skin = container->skin;
	bool exist = MyGUI::SkinManager::getInstance().isExist(container->skin);
	if ( !exist && !container->skin.empty())
	{
		skin = WidgetTypes::getInstance().find(container->type)->default_skin;
		std::string tmp;
		if (skin.empty()) tmp = "empty skin";
		else  tmp = "'" + skin + "'";
		// FIXME : not translated string
		std::string mess = MyGUI::utility::toString("'", container->skin, "' skin not found , temporary changed to ", tmp);
		GroupMessage::getInstance().addMessage(mess, MyGUI::LogLevel::Error);
	}

	if (nullptr == _parent)
	{
		std::string layer = DEFAULT_EDITOR_LAYER;
		if (_test && MyGUI::LayerManager::getInstance().isExist(container->layer))
			layer = container->layer;
		container->widget = MyGUI::Gui::getInstance().createWidgetT(container->type, skin, coord, align, layer, tmpname);
	}
	else
	{
		container->widget = _parent->createWidgetT(container->type, skin, coord, align, tmpname);
	}

	add(container);

	// берем детей и крутимся
	MyGUI::xml::ElementEnumerator widget = _widget->getElementEnumerator();
	while (widget.next())
	{
		std::string key, value, type;

		if (widget->getName() == "Widget") parseWidget(widget, container->widget, _test);
		else if (widget->getName() == "Property")
		{
			// парсим атрибуты
			if (!widget->findAttribute("key", key)) continue;
			if (!widget->findAttribute("value", value)) continue;

			// и пытаемся парсить свойство
			if ( tryToApplyProperty(container->widget, key, value, _test) == false ) continue;

			container->mProperty.push_back(MyGUI::PairString(key, value));
		}
		else if (widget->getName() == "UserString")
		{
			// парсим атрибуты
			if (!widget->findAttribute("key", key)) continue;
			if (!widget->findAttribute("value", value)) continue;
			//container->mUserString.insert(MyGUI::PairString(key, value));
			container->mUserString.push_back(MyGUI::PairString(key, value));
		}
		else if (widget->getName() == "Controller")
		{
			// парсим атрибуты
			if (!widget->findAttribute("type", type)) continue;
			ControllerInfo* controllerInfo = new ControllerInfo();
			controllerInfo->mType = type;

			MyGUI::ControllerItem* item = nullptr;
			if (_test)
			{
				item = MyGUI::ControllerManager::getInstance().createItem(type);
			}

			MyGUI::xml::ElementEnumerator prop = widget->getElementEnumerator();
			while (prop.next("Property"))
			{
				if (!prop->findAttribute("key", key)) continue;
				if (!prop->findAttribute("value", value)) continue;
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

bool EditorWidgets::tryToApplyProperty(MyGUI::Widget* _widget, const std::string& _key, const std::string& _value, bool _test)
{
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

		if (_test || std::find(ignore_parameters.begin(), ignore_parameters.end(), _key) == ignore_parameters.end())
		{
			// для поддержки старых пропертей
#ifndef MYGUI_DONT_USE_OBSOLETE
			MyGUI::WidgetManager::getInstance()._parse(_widget, _key, _value);
#else
			_widget->setProperty(_key, _value);
#endif
		}
		//FIXME вроде уже не нужно, без этой строки все работает намного быстрее из-за корректных ленивых вычислеиний
		//Ogre::Root::getSingleton().renderOneFrame();
	}
	catch(...)
	{
		GroupMessage::getInstance().addMessage("Can't apply '" + _key + "'property.", MyGUI::LogLevel::Error);
		return false;
	}
	return true;
}

void EditorWidgets::serialiseWidget(WidgetContainer * _container, MyGUI::xml::ElementPtr _node)
{
	MyGUI::xml::ElementPtr node = _node->createChild("Widget");

	node->addAttribute("type", _container->type);
	node->addAttribute("skin", _container->skin);
	if (!_container->relative_mode) node->addAttribute("position", _container->position());
	else node->addAttribute("position_real", _container->position(false));
	if ("" != _container->align) node->addAttribute("align", _container->align);
	if ("" != _container->layer) node->addAttribute("layer", _container->layer);
	if ("" != _container->name) node->addAttribute("name", _container->name);

	for (MyGUI::VectorStringPairs::iterator iter = _container->mProperty.begin(); iter != _container->mProperty.end(); ++iter)
	{
		MyGUI::xml::ElementPtr nodeProp = node->createChild("Property");
		nodeProp->addAttribute("key", iter->first);
		nodeProp->addAttribute("value", iter->second);
	}

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
		serialiseWidget(*iter, node);
	}
}

void EditorWidgets::loadIgnoreParameters(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
{
	MyGUI::xml::ElementEnumerator parameter = _node->getElementEnumerator();
	while (parameter.next("Parameter"))
	{
		std::string name = parameter->findAttribute("key");
		ignore_parameters.push_back(name);
	}
}
