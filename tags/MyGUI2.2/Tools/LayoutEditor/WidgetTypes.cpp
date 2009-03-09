#include "precompiled.h"
#include "WidgetTypes.h"

const std::string LogSection = "LayoutEditor";

MYGUI_INSTANCE_IMPLEMENT(WidgetTypes);

void WidgetTypes::initialise()
{
	//loadTypes();
	MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("Widgets") = MyGUI::newDelegate(this, &WidgetTypes::loadWidgets);
	MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("Values") = MyGUI::newDelegate(this, &WidgetTypes::loadValues);
}

void WidgetTypes::shutdown()
{
	for (std::vector<WidgetStyle*>::iterator iter = widget_types.begin(); iter != widget_types.end(); ++iter) delete *iter;
	widget_types.clear();
	for (std::vector<PossibleValue*>::iterator iter = possible_values.begin(); iter != possible_values.end(); ++iter) delete *iter;
	possible_values.clear();
}

WidgetStyle* WidgetTypes::find(std::string _type)
{
	for (std::vector<WidgetStyle*>::iterator iter = widget_types.begin(); iter != widget_types.end(); ++iter)
	{
		if ((*iter)->name == _type)
		{
			return *iter;
		}
	}
	return find("Widget");
}

std::vector<std::string> WidgetTypes::findPossibleValues(std::string _name)
{
	for (std::vector<PossibleValue*>::iterator iter = possible_values.begin(); iter != possible_values.end(); ++iter)
	{
		if ((*iter)->name == _name)
		{
			return (*iter)->values;
		}
	}
	return std::vector<std::string>();
}

/*void WidgetTypes::loadTypes()
{
	std::string _fileName = "widgets.xml";
	std::string _instance = "Editor";

	MyGUI::xml::Document doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty()) {
		MYGUI_LOGGING(LogSection, Error, _instance << " : '" << _fileName << "' not found");
		return;
	}
	if (false == doc.open(file)) {
		MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return;
	}

	MyGUI::xml::ElementPtr root = doc.getRoot();
	if ( (nullptr == root) || (root->getName() != "MyGUI") ) {
		MYGUI_LOGGING(LogSection, Error, _instance << " : '" << _fileName << "', tag 'MyGUI' not found");
		return;
	}

	std::string type;
	if (root->findAttribute("type", type)) {
		if (type == "Widgets")
		{
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator widget = root->getElementEnumerator();
			while (widget.next("Widget")) parseWidgetType(widget);
			MyGUI::xml::ElementEnumerator value = root->getElementEnumerator();
			while (value.next("Value")) parseValue(value);
		}
	}
}*/

WidgetStyle * WidgetTypes::getWidgetType(const std::string & _name)
{
	// ищем тип, если нет, то создаем
	for (VectorWidgetType::iterator iter=widget_types.begin(); iter!=widget_types.end(); ++iter) {
		if ((*iter)->name == _name) return (*iter);
	}

	WidgetStyle * type = new WidgetStyle(_name);
	widget_types.push_back(type);

	return type;
}

void WidgetTypes::loadWidgets(MyGUI::xml::ElementPtr _node, const std::string & _file, MyGUI::Version _version)
{
	MyGUI::xml::ElementEnumerator widgets = _node->getElementEnumerator();
	while (widgets.next("Widget")) {

		WidgetStyle * widget_type = getWidgetType(widgets->findAttribute("name"));

		// берем детей и крутимся
		MyGUI::xml::ElementEnumerator field = widgets->getElementEnumerator();
		while (field.next()) {

			std::string key, value, group;

			if (field->getName() == "Property") {
				if (false == field->findAttribute("key", key)) continue;
				if (false == field->findAttribute("value", value)) continue;
				field->findAttribute("group", group);
				if (key == "Skin")
				{
					if (group.empty()) group = DEFAULT_GOROUP_NAME;
					skin_groups[group].push_back(std::make_pair(value, widget_type->name));
					widget_type->skin.push_back(value);
				}
				else if (key == "DefaultSkin") widget_type->default_skin = value;
				else if (key == "Parent") widget_type->parent = MyGUI::utility::parseBool(value);
				else if (key == "Child") widget_type->child = MyGUI::utility::parseBool(value);
				else if (key == "Resizeable") widget_type->resizeable = MyGUI::utility::parseBool(value);
				else if (key == "ItemManager") widget_type->many_items = MyGUI::utility::parseBool(value);
			}
			else if (field->getName() == "Parameter") {
				if (false == field->findAttribute("key", key)) continue;
				if (false == field->findAttribute("value", value)) continue;
				//widget_type->parameter.insert(std::make_pair(key, value));
				widget_type->parameter.push_back(std::make_pair(key, value));
			}
		}
	}
}

PossibleValue * WidgetTypes::getPossibleValue(const std::string & _name)
{

	PossibleValue * possible_value = nullptr;
	for (std::vector<PossibleValue*>::iterator iter=possible_values.begin(); iter!=possible_values.end(); ++iter) {
		if ((*iter)->name == _name) {
			return (*iter);
		}
	}

	if (possible_value == nullptr) {
		possible_value = new PossibleValue();
		possible_value->name = _name;
		possible_values.push_back(possible_value);
	}

	return possible_value;
}

void WidgetTypes::loadValues(MyGUI::xml::ElementPtr _node, const std::string & _file, MyGUI::Version _version)
{
	MyGUI::xml::ElementEnumerator widgets = _node->getElementEnumerator();
	while (widgets.next("Value")) {

		std::string name = widgets->findAttribute("name");
		PossibleValue * possible_value = getPossibleValue(name);

		// тип мерджа переменных
		std::string merge = widgets->findAttribute("merge");
		// дополняем своими данными, по дефолту
		if (merge == "add") {
		}
		// удаляем и добавляем свои
		else if (merge == "replace") {
			possible_value->values.clear();
		}

		// берем детей и крутимся
		MyGUI::xml::ElementEnumerator field = widgets->getElementEnumerator();
		while (field.next()) {

			std::string key, value;

			if (field->getName() == "Property") {
				if (false == field->findAttribute("key", key)) continue;
				possible_value->values.push_back(MyGUI::LanguageManager::getInstance().replaceTags(key));
			}

		}
	}
}
