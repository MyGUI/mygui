#include "MyGUI.h"

typedef std::map<std::string, std::string> MapString;

struct WidgetType{
	WidgetType(): parent(false), child(true), resizeable(true), many_strings(false) {}
	std::string name;
	std::vector<std::string> skin;
	MapString parameter;
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
private:
	void loadTypes();
	void parseWidgetType(MyGUI::xml::xmlNodeIterator _widget);
	void parseValue(MyGUI::xml::xmlNodeIterator _value);
};
