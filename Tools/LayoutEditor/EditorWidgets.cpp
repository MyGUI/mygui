#include "precompiled.h"
#include "EditorWidgets.h"
#include "BasisManager.h"
#include "WidgetTypes.h"

const std::string LogSection = "LayoutEditor";

MYGUI_INSTANCE_IMPLEMENT(EditorWidgets);

inline const Ogre::UTFString localise(const Ogre::UTFString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

void MapSet(StringPairs & _map, const std::string &_key, const std::string &_value)
{
	bool find = false;
	for (StringPairs::iterator iter=_map.begin(); iter!=_map.end(); ++iter) {
		if (iter->first == _key) {
			iter->second = _value;
			find = true;
		}
	}
	if (!find) {
		_map.push_back(std::make_pair(_key, _value));
	}
}

StringPairs::iterator MapFind(StringPairs & _map, const std::string &_key)
{
	StringPairs::iterator iter = _map.begin();
	for (; iter!=_map.end(); ++iter) {
		if (iter->first == _key) break;
	}
	return iter;
}

void MapErase(StringPairs & _map, const std::string &_key)
{
	for (StringPairs::iterator iter = _map.begin(); iter!=_map.end(); ++iter) {
		if (iter->first == _key) {
			_map.erase(iter);
			return;
		}
	}
}

MyGUI::IntCoord convertCoordToParentCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget)
{
	MyGUI::WidgetPtr parent = widget->getParent();
	while (nullptr != parent){
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

bool EditorWidgets::load(std::string _fileName)
{
	std::string _instance = "Editor";

	MyGUI::xml::Document doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty())
	{
		if (false == doc.open(_fileName)) {
			MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
			return false;
		}
	}
	else if (false == doc.open(file))
	{
		MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return false;
	}

	MyGUI::xml::ElementPtr root = doc.getRoot();
	if ( (nullptr == root) || (root->getName() != "MyGUI") ) {
		MYGUI_LOGGING(LogSection, Error, _instance << " : '" << _fileName << "', tag 'MyGUI' not found");
		return false;
	}

	std::string type;
	if (root->findAttribute("type", type)) {
		if (type == "Layout")
		{
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator widget = root->getElementEnumerator();
			while (widget.next("Widget")) parseWidget(widget, 0);
		}

	}

	widgets_changed = true;
	return true;
}

bool EditorWidgets::save(std::string _fileName)
{
	std::string _instance = "Editor";

	MyGUI::xml::Document doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty()) {
		file = _fileName;
	}

	doc.createDeclaration();
	MyGUI::xml::ElementPtr root = doc.createRoot("MyGUI");
	root->addAttribute("type", "Layout");

	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		// в корень только сирот
		if (nullptr == (*iter)->widget->getParent()) serialiseWidget(*iter, root);
	}

	if (false == doc.save(file)) {
		MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return false;
	}

	return true;
}

void EditorWidgets::loadxmlDocument(MyGUI::xml::Document * doc, bool _test)
{
	MyGUI::xml::ElementPtr root = doc->getRoot();

	std::string type;
	if (root->findAttribute("type", type)) {
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
		widgets.push_back(_container);
	}
	else
	{
		MyGUI::WidgetPtr parent = _container->widget->getParent();
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

void EditorWidgets::remove(MyGUI::WidgetPtr _widget)
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
			MyGUI::WidgetPtr parent = _container->widget->getParent();
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

WidgetContainer * EditorWidgets::find(MyGUI::WidgetPtr _widget)
{
	return _find(_widget, "", widgets);
}

WidgetContainer * EditorWidgets::find(std::string _name)
{
	return _find(NULL, _name, widgets);
}

WidgetContainer * EditorWidgets::_find(MyGUI::WidgetPtr _widget, std::string _name, std::vector<WidgetContainer*> _widgets)
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

void EditorWidgets::parseWidget(MyGUI::xml::ElementEnumerator & _widget, MyGUI::WidgetPtr _parent, bool _test)
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
		coord = MyGUI::WidgetManager::getInstance().convertRelativeToInt(MyGUI::FloatCoord::parse(position), _parent);
	}

	// в гуе на 2 одноименных виджета ругается и падает, а у нас будет просто переименовывать
	if (false == container->name.empty())
	{
		WidgetContainer * iter = find(container->name);
		if (nullptr != iter)
		{
			static long renameN=0;
			std::string mess = MyGUI::utility::toString("widget with same name name '", container->name, "'. Renamed to '", container->name, renameN, "'.");
			MYGUI_LOGGING(LogSection, Warning, mess);
			MyGUI::MessagePtr message = MyGUI::Message::createMessageBox("Message", localise("Warning"), mess, MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok, "Overlapped");
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
	bool exist = MyGUI::SkinManager::getInstance().isExist(container->skin);
	if ( !exist )
	{
		skin = WidgetTypes::getInstance().find(container->type)->default_skin;
		std::string mess = MyGUI::utility::toString("'", container->skin, "' skin not found , temporary changed to '", skin, "'");
		MyGUI::MessagePtr message = MyGUI::Message::createMessageBox("Message", "Error", mess , MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok, "Overlapped");
	}

	if (nullptr == _parent) {
		container->widget = MyGUI::Gui::getInstance().createWidgetT(container->type, skin, coord, align, DEFAULT_EDITOR_LAYER/*layer*/, tmpname);
	}
	else
	{
		container->widget = _parent->createWidgetT(container->type, skin, coord, align, tmpname);
	}

	add(container);

	// берем детей и крутимся
	MyGUI::xml::ElementEnumerator widget = _widget->getElementEnumerator();
	while (widget.next()) {

		std::string key, value;

		if (widget->getName() == "Widget") parseWidget(widget, container->widget);
		else if (widget->getName() == "Property") {

			// парсим атрибуты
			if (false == widget->findAttribute("key", key)) continue;
			if (false == widget->findAttribute("value", value)) continue;

			// и пытаемся парсить свойство
			if ( tryToApplyProperty(container->widget, key, value, _test) == false ) continue;

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

bool EditorWidgets::tryToApplyProperty(MyGUI::WidgetPtr _widget, std::string _key, std::string _value, bool _test)
{
 	try {

		if (_key == "Image_Texture")
		{
			if (!MyGUI::helper::isFileExist(_value)) {
				MyGUI::MessagePtr message = MyGUI::Message::createMessageBox("Message", localise("Warning"), "No such " + _key + ": '" + _value + "'. This value will be saved.", MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok, "Overlapped");
				return true;
			}
			/*if ( false == Ogre::TextureManager::getSingleton().resourceExists(_value) ) {
				MyGUI::Message::_createMessage(localise("Warning"), "No such " + _key + ": '" + _value + "'. This value will be saved.", "", "Overlapped", true, nullptr, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
				return true;
			}*/
		}

		if (_test || std::find(ignore_parameters.begin(), ignore_parameters.end(), _key) == ignore_parameters.end())
			MyGUI::WidgetManager::getInstance().parse(_widget, _key, _value);
		//FIXME вроде уже не нужно, без этой строки все работает намного быстрее из-за корректных ленивых вычислеиний
		//Ogre::Root::getSingleton().renderOneFrame();
	}
	catch(MyGUI::MyGUIException & e)
	{
		MyGUI::MessagePtr message = MyGUI::Message::createMessageBox("Message", localise("Warning"), "Can't apply '" + _key + "'property" + ": " + e.getDescription() + ". This value will be saved.", MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok, "Overlapped");
	}
	catch(Ogre::Exception & )
	{
		MyGUI::MessagePtr message = MyGUI::Message::createMessageBox("Message", localise("Warning"), "No such " + _key + ": '" + _value + "'. This value will be saved.", MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok, "Overlapped");
	}// for incorrect meshes or textures
	catch(...)
	{
		MyGUI::MessagePtr message = MyGUI::Message::createMessageBox("Message", "Error", "Can't apply '" + _key + "'property.", MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok, "Overlapped");
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

	for (StringPairs::iterator iter = _container->mProperty.begin(); iter != _container->mProperty.end(); ++iter)
	{
		MyGUI::xml::ElementPtr nodeProp = node->createChild("Property");
		nodeProp->addAttribute("key", iter->first);
		nodeProp->addAttribute("value", iter->second);
	}

	for (StringPairs::iterator iter = _container->mUserString.begin(); iter != _container->mUserString.end(); ++iter)
	{
		MyGUI::xml::ElementPtr nodeProp = node->createChild("UserString");
		nodeProp->addAttribute("key", iter->first);
		nodeProp->addAttribute("value", iter->second);
	}

	for (std::vector<WidgetContainer*>::iterator iter = _container->childContainers.begin(); iter != _container->childContainers.end(); ++iter)
	{
		serialiseWidget(*iter, node);
	}
}

void EditorWidgets::loadIgnoreParameters(MyGUI::xml::ElementPtr _node, const std::string & _file, MyGUI::Version _version)
{
	MyGUI::xml::ElementEnumerator parameter = _node->getElementEnumerator();
	while (parameter.next("Parameter")) {
		std::string name = parameter->findAttribute("key");
		ignore_parameters.push_back(name);
	}
}
