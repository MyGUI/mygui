#include "WidgetContainer.h"
#include "BasisManager.h"
#include "WidgetTypes.h"

const std::string LogSection = "LayoutEditor";

INSTANCE_IMPLEMENT(EditorWidgets);

void EditorWidgets::initialise()
{
	global_counter = 0;
}

void EditorWidgets::shutdown()
{
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter) delete *iter;
	widgets.clear();
}

bool EditorWidgets::load(std::string _fileName)
{
	std::string _instance = "Editor";

	MyGUI::xml::xmlDocument doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty())
	{
		if (false == doc.open(_fileName)) {
			LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
			return false;
		}
	}
	else if (false == doc.open(file))
	{
		LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return false;
	}

	MyGUI::xml::xmlNodePtr root = doc.getRoot();
	if ( (null == root) || (root->getName() != "MyGUI") ) {
		LOGGING(LogSection, Error, _instance << " : '" << _fileName << "', tag 'MyGUI' not found");
		return false;
	}

	std::string type;
	if (root->findAttribute("type", type)) {
		if (type == "Layout")
		{
			// берем детей и крутимся
			MyGUI::xml::xmlNodeIterator widget = root->getNodeIterator();
			while (widget.nextNode("Widget")) parseWidget(widget, 0);
		}
		
	}
	return true;
}

bool EditorWidgets::save(std::string _fileName)
{
	std::string _instance = "Editor";

	MyGUI::xml::xmlDocument doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty()) {
		file = _fileName;
	}

	doc.createInfo();
	MyGUI::xml::xmlNodePtr root = doc.createRoot("MyGUI");
	root->addAttributes("type", "Layout");

	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		// в корень только сирот
		if (null == (*iter)->widget->getParent()) serialiseWidget(*iter, root);
	}

	if (false == doc.save(file)) {
		LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return false;
	}

	return true;
}

void EditorWidgets::loadxmlDocument(MyGUI::xml::xmlDocument * doc, bool _test)
{
	MyGUI::xml::xmlNodePtr root = doc->getRoot();

	std::string type;
	if (root->findAttribute("type", type)) {
		if (type == "Layout")
		{
			// берем детей и крутимся
			MyGUI::xml::xmlNodeIterator widget = root->getNodeIterator();
			while (widget.nextNode("Widget")) parseWidget(widget, 0, _test);
		}
	}
}

MyGUI::xml::xmlDocument * EditorWidgets::savexmlDocument()
{
	MyGUI::xml::xmlDocument * doc = new MyGUI::xml::xmlDocument();

	doc->createInfo();
	MyGUI::xml::xmlNodePtr root = doc->createRoot("MyGUI");
	root->addAttributes("type", "Layout");

	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		// в корень только сирот
		if (null == (*iter)->widget->getParent()) serialiseWidget(*iter, root);
	}

	return doc;
}

void EditorWidgets::add(WidgetContainer * _container)
{
	widgets.push_back(_container);
}

void EditorWidgets::remove(MyGUI::WidgetPtr _widget)
{
	// дети вперед
	MyGUI::VectorWidgetPtr childs = _widget->getChilds();
	for (MyGUI::VectorWidgetPtr::iterator iter = childs.begin(); iter != childs.end(); ++iter)
	{
		if (null != find(*iter)) remove(*iter);
	}
	WidgetContainer * _container = find(_widget);

	MyGUI::Gui::getInstance().destroyWidget(_widget);

	if (null != _container)
	{
		widgets.erase(std::find(widgets.begin(), widgets.end(), _container));
		delete _container;
	}
}

void EditorWidgets::clear()
{
	while (!widgets.empty())
	{
		remove(widgets[widgets.size()-1]->widget);
	}
	global_counter = 0;
}

WidgetContainer * EditorWidgets::find(MyGUI::WidgetPtr _widget)
{
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		if ((*iter)->widget == _widget)
		{
			return *iter;
		}
	}
	return null;
}
WidgetContainer * EditorWidgets::find(std::string _name)
{
	if (_name.empty()) return null;
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		if ((*iter)->name == _name)
		{
			return *iter;
		}
	}
	return null;
}

void EditorWidgets::parseWidget(MyGUI::xml::xmlNodeIterator & _widget, MyGUI::WidgetPtr _parent, bool _test)
{
	WidgetContainer * container = new WidgetContainer();
	// парсим атрибуты виджета
	MyGUI::IntCoord coord;
	MyGUI::Align align = MyGUI::ALIGN_DEFAULT;
	std::string position;
	std::string layer;

	_widget->findAttribute("name", container->name);
	_widget->findAttribute("type", container->type);
	_widget->findAttribute("skin", container->skin);
	_widget->findAttribute("layer", layer);
	if (_widget->findAttribute("align", container->align)) align = MyGUI::SkinManager::parseAlign(container->align);
	if (_widget->findAttribute("position", position)) coord = MyGUI::IntCoord::parse(position);
	if (_widget->findAttribute("position_real", position))
	{
		container->relative_mode = true;
		coord = MyGUI::Gui::getInstance().convertRelativeToInt(MyGUI::FloatCoord::parse(position), _parent);
	}

	// в гуе на 2 одноименных виджета ругается и падает, а у нас будет просто переименовывать
	if (false == container->name.empty())
	{
		WidgetContainer * iter = find(container->name);
		if (null != iter)
		{
			static long renameN=0;
			std::string mess = MyGUI::utility::toString("widget with same name name '", container->name, "'. Renamed to '", container->name, renameN, "'.");
			LOGGING(LogSection, Warning, mess);
			MyGUI::Message::_createMessage("Warning", mess, "", "LayoutEditor_Overlapped", true, null, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
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

	if (container->skin.empty()) container->skin = "Default";

	// проверяем скин на присутствие
	std::string skin = container->skin;
	bool exist = MyGUI::SkinManager::getInstance().isSkinExist(container->skin);
	if ( !exist )
   {
		skin = WidgetTypes::getInstance().find(container->type)->default_skin;
		std::string mess = MyGUI::utility::toString("skin not found '", container->skin, "' , change on '", skin, "'");
		MyGUI::Message::_createMessage("Error", mess , "", "LayoutEditor_Overlapped", true, null, MyGUI::Message::IconError | MyGUI::Message::Ok);
	}

	if (null == _parent) {
		container->widget = MyGUI::Gui::getInstance().createWidgetT(container->type, skin, coord, align, layer, tmpname);
		add(container);
	}
	else
	{
		container->widget = _parent->createWidgetT(container->type, skin, coord, align, tmpname);
		add(container);
	}

	// берем детей и крутимся
	MyGUI::xml::xmlNodeIterator widget = _widget->getNodeIterator();
	while (widget.nextNode()) {

		std::string key, value;

		if (widget->getName() == "Widget") parseWidget(widget, container->widget);
		else if (widget->getName() == "Property") {

			// парсим атрибуты
			if (false == widget->findAttribute("key", key)) continue;
			if (false == widget->findAttribute("value", value)) continue;

			// и парсим свойство
			try{
				if (_test || 
					 (("Message_Modal" != key) && ("Window_AutoAlpha" != key) && ("Window_Snap" != key)))
					MyGUI::WidgetManager::getInstance().parse(container->widget, key, value);
				Ogre::Root::getSingleton().renderOneFrame();
			}
			catch(...)
			{
				MyGUI::Message::_createMessage("Warning", "No such " + key + ": '" + value + "'", "", "LayoutEditor_Overlapped", true, null, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
				if (key == "Image_Texture") MyGUI::WidgetManager::getInstance().parse(container->widget, key, "");
			}// for incorrect meshes or textures

			container->mProperty.push_back(std::make_pair(key, value));
		}
		else if (widget->getName() == "UserString") {
			// парсим атрибуты
			if (false == widget->findAttribute("key", key)) continue;
			if (false == widget->findAttribute("value", value)) continue;
			//container->mUserString.insert(std::make_pair(key, value));
			container->mUserString.push_back(std::make_pair(key, value));
		}

	};
}

void EditorWidgets::serialiseWidget(WidgetContainer * _container, MyGUI::xml::xmlNodePtr _node)
{
	MyGUI::xml::xmlNodePtr node = _node->createChild("Widget");

	node->addAttributes("type", _container->type);
	node->addAttributes("skin", _container->skin);
	if (!_container->relative_mode) node->addAttributes("position", _container->position());
	else node->addAttributes("position_real", _container->position(false));
	if ("" != _container->align) node->addAttributes("align", _container->align);
	if ("" != _container->layer()) node->addAttributes("layer", _container->layer());
	if ("" != _container->name) node->addAttributes("name", _container->name);

	for (StringPairs::iterator iter = _container->mProperty.begin(); iter != _container->mProperty.end(); ++iter)
	{
		MyGUI::xml::xmlNodePtr nodeProp = node->createChild("Property");
		nodeProp->addAttributes("key", iter->first);
		nodeProp->addAttributes("value", iter->second);
	}

	for (StringPairs::iterator iter = _container->mUserString.begin(); iter != _container->mUserString.end(); ++iter)
	{
		MyGUI::xml::xmlNodePtr nodeProp = node->createChild("UserString");
		nodeProp->addAttributes("key", iter->first);
		nodeProp->addAttributes("value", iter->second);
	}

	// метод медленный, т.к. квадратичная сложность
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		MyGUI::WidgetPtr parent = (*iter)->widget->getParent();

		// сынок - это ты?
		if ((_container->widget->getWidgetType() == "Window") && (_container->widget->getClientWidget() != null)) {
			if (null != parent) {
				if (_container->widget == parent->getParent()) serialiseWidget(*iter, node);
			}
		}
		else if (_container->widget == parent) {
			serialiseWidget(*iter, node);
		}
	}
}
