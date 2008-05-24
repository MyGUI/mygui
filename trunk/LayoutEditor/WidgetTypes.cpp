#include "WidgetTypes.h"

const std::string LogSection = "LayoutEditor";

INSTANCE_IMPLEMENT(WidgetTypes);

void WidgetTypes::initialise()
{
	loadTypes();
}

void WidgetTypes::shutdown()
{
	for (std::vector<WidgetType*>::iterator iter = widget_types.begin(); iter != widget_types.end(); ++iter) delete *iter;
	widget_types.clear();
	for (std::vector<PossibleValue*>::iterator iter = possible_values.begin(); iter != possible_values.end(); ++iter) delete *iter;
	possible_values.clear();
}

WidgetType* WidgetTypes::find(std::string _type)
{
	for (std::vector<WidgetType*>::iterator iter = widget_types.begin(); iter != widget_types.end(); ++iter)
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

void WidgetTypes::loadTypes()
{
	std::string _fileName = "widgets.xml";
	std::string _instance = "Editor";

	MyGUI::xml::xmlDocument doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty()) {
		LOGGING(LogSection, Error, _instance << " : '" << _fileName << "' not found");
		return;
	}
	if (false == doc.open(file)) {
		LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return;
	}

	MyGUI::xml::xmlNodePtr root = doc.getRoot();
	if ( (null == root) || (root->getName() != "MyGUI") ) {
		LOGGING(LogSection, Error, _instance << " : '" << _fileName << "', tag 'MyGUI' not found");
		return;
	}

	std::string type;
	if (root->findAttribute("type", type)) {
		if (type == "Widgets")
		{
			// берем детей и крутимся
			MyGUI::xml::xmlNodeIterator widget = root->getNodeIterator();
			while (widget.nextNode("Widget")) parseWidgetType(widget);
			MyGUI::xml::xmlNodeIterator value = root->getNodeIterator();
			while (value.nextNode("Value")) parseValue(value);
		}
	}
}

void WidgetTypes::parseWidgetType(MyGUI::xml::xmlNodeIterator _widget)
{
	WidgetType * widget_type = new WidgetType();
	// парсим атрибуты виджета
	_widget->findAttribute("name", widget_type->name);

	widget_types.push_back(widget_type);

	// берем детей и крутимся
	MyGUI::xml::xmlNodeIterator field = _widget->getNodeIterator();
	while (field.nextNode()) {

		std::string key, value, group;

		if (field->getName() == "Property") {
			if (false == field->findAttribute("key", key)) continue;
			if (false == field->findAttribute("value", value)) continue;
			field->findAttribute("group", group);
			if (key == "Skin")
			{
				if (group.empty()) group = "Default";
				skin_groups[group].push_back(std::make_pair(value, widget_type->name));
				widget_type->skin.push_back(value);
			}
			else if (key == "Parent") widget_type->parent = MyGUI::utility::parseBool(value);
			else if (key == "Child") widget_type->child = MyGUI::utility::parseBool(value);
			else if (key == "Resizeable") widget_type->resizeable = MyGUI::utility::parseBool(value);
			else if (key == "StringManager") widget_type->many_strings = MyGUI::utility::parseBool(value);
		}
		else if (field->getName() == "Parameter") {
			if (false == field->findAttribute("key", key)) continue;
			if (false == field->findAttribute("value", value)) continue;
			//widget_type->parameter.insert(std::make_pair(key, value));
			widget_type->parameter.push_back(std::make_pair(key, value));
		}
	}
}

void WidgetTypes::parseValue(MyGUI::xml::xmlNodeIterator _value)
{
	PossibleValue * possible_value = new PossibleValue();
	// парсим атрибуты виджета
	_value->findAttribute("name", possible_value->name);

	possible_values.push_back(possible_value);

	// берем детей и крутимся
	MyGUI::xml::xmlNodeIterator field = _value->getNodeIterator();
	while (field.nextNode()) {

		std::string key, value;

		if (field->getName() == "Property") {
			if (false == field->findAttribute("key", key)) continue;
			possible_value->values.push_back(key);
		}

	}
}
