#ifndef __WIDGET_TYPES_H__
#define __WIDGET_TYPES_H__

typedef std::vector<std::pair<Ogre::String, Ogre::String> > StringPairs;
//typedef std::map<Ogre::String, Ogre::String> MapString;
//typedef std::vector<std::pair<std::string, std::string> > MapString;
typedef std::map< std::string, StringPairs > SkinGroups;

const std::string DEFAULT_GOROUP_NAME = "Default";

struct WidgetStyle{
	WidgetStyle(const std::string & _name) : name(_name), parent(false), child(true), resizeable(true), many_items(false) {}
	std::string name;
	std::vector<std::string> skin;
	std::string default_skin;
	StringPairs parameter;
	// can have child widgets
	bool parent;
	// can have farent
	bool child;
	// can be selected
	bool resizeable;
	// have many items (such as List, ComboBox, Tab, MenuBar, etc...)
	bool many_items;
};

struct PossibleValue{
	std::string name;
	std::vector<std::string> values;
};

class WidgetTypes{
	MYGUI_INSTANCE_HEADER(WidgetTypes);
public:
	void initialise();
	void shutdown();
	WidgetStyle* find(std::string _type);
	std::vector<std::string> findPossibleValues(std::string _name);

	typedef std::vector<WidgetStyle*> VectorWidgetType;
	VectorWidgetType widget_types;

	std::vector<PossibleValue*> possible_values;
	SkinGroups skin_groups;
private:
	//void loadTypes();
	void loadWidgets(MyGUI::xml::ElementPtr _node, const std::string & _file, MyGUI::Version _version);
	void loadValues(MyGUI::xml::ElementPtr _node, const std::string & _file, MyGUI::Version _version);

	WidgetStyle * getWidgetType(const std::string & _name);

	PossibleValue * getPossibleValue(const std::string & _name);

};

#endif // __WIDGET_TYPES_H__
