#include "MyGUI.h"

typedef std::vector<std::pair<Ogre::String, Ogre::String> > StringPairs;
//typedef std::map<Ogre::String, Ogre::String> MapString;
//typedef std::vector<std::pair<std::string, std::string> > MapString;
typedef std::map< std::string, StringPairs > SkinGroups;

const std::string DEFAULT_GOROUP_NAME = "Default";

struct WidgetType{
	WidgetType(const std::string & _name) : name(_name), parent(false), child(true), resizeable(true), many_items(false) {}
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
	INSTANCE_HEADER(WidgetTypes);
public:
	void initialise();
	void shutdown();
	WidgetType* find(std::string _type);
	std::vector<std::string> findPossibleValues(std::string _name);

	typedef std::vector<WidgetType*> VectorWidgetType;
	VectorWidgetType widget_types;

	std::vector<PossibleValue*> possible_values;
	SkinGroups skin_groups;
private:
	//void loadTypes();
	void loadWidgets(MyGUI::xml::xmlNodePtr _node, const std::string & _file);
	void loadValues(MyGUI::xml::xmlNodePtr _node, const std::string & _file);

	WidgetType * getWidgetType(const std::string & _name);

	PossibleValue * getPassibleValue(const std::string & _name);

};
