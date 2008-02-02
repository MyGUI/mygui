#ifndef __WIDGET_CONTAINER_H__
#define __WIDGET_CONTAINER_H__

#include <vector>
#include "MyGUI.h"

typedef std::vector<std::pair<std::string, std::string>> StringPairs;
typedef std::map<std::string, std::string> MapString;

struct WidgetContainer
{
	WidgetContainer(){}
	WidgetContainer(std::string _container_name, std::string _type, std::string _skin, MyGUI::WidgetPtr _widget, std::string _name = ""):
		container_name(_container_name),
		type(_type),
		skin(_skin),
		widget(_widget),
		name()
	{}

	std::string container_name;
	MyGUI::WidgetPtr widget;
	StringPairs mProperty;
	MapString mUserString;
	std::string type;	// necessary
	std::string skin;	// necessary (except Sheet)
	std::string position; // necessary this or position_real (except Sheet)
	std::string position_real; // necessary this or position (except Sheet)
	std::string layer; // necessary for root widgets
	std::string align; // not necessary
	std::string name; // not necessary
};

class EditorWidgets{

	INSTANCE_HEADER(EditorWidgets);

public:
	void initialise();
	void shutdown();
	bool load(std::string _fileName);
	bool save(std::string _fileName);
	WidgetContainer * find(MyGUI::WidgetPtr _widget);
	WidgetContainer * find(std::string _name);
	void add(WidgetContainer * _container);
private:
	void parseWidget(MyGUI::xml::xmlNodeIterator & _widget, MyGUI::WidgetPtr _parent);
	void serialiseWidget(WidgetContainer * _container, MyGUI::xml::xmlNodePtr _node);

	std::vector<WidgetContainer*> widgets;
};

#endif // __WIDGET_CONTAINER_H__