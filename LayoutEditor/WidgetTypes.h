#include "MyGUI.h"

typedef std::vector<std::pair<Ogre::String, Ogre::String> > StringPairs;
//typedef std::map<Ogre::String, Ogre::String> MapString;
//typedef std::vector<std::pair<std::string, std::string> > MapString;
typedef std::map< std::string, StringPairs > SkinGroups;

struct WidgetType{
	WidgetType(): parent(false), child(true), resizeable(true), many_strings(false) {}
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
	// have many strings (such as List, ComboBox, Tab, etc...)
	bool many_strings;
};

struct PossibleValue{
	std::string name;
	std::vector<std::string> values;
};

class WidgetTypes{
	INSTANCE_HEADER(WidgetTypes);
public:
	void initialise();
	void shutdown();
	WidgetType* find(std::string _type);
	std::vector<std::string> findPossibleValues(std::string _name);
	std::vector<WidgetType*> widget_types;
	std::vector<PossibleValue*> possible_values;
	SkinGroups skin_groups;
private:
	void loadTypes();
	void parseWidgetType(MyGUI::xml::xmlNodeIterator _widget);
	void parseValue(MyGUI::xml::xmlNodeIterator _value);
};
