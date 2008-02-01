#ifndef __WIDGET_CONTAINER_H__
#define __WIDGET_CONTAINER_H__

#include <vector>
#include "MyGUI.h"

typedef std::vector<std::pair<std::string, std::string>> StringPairs;
typedef std::map<std::string, std::string> MapString;

struct WidgetContainer
{
	WidgetContainer(){}
	WidgetContainer(std::string _container_name, MyGUI::WidgetPtr _widget):
		container_name(_container_name),
		widget(_widget)
	{}

	std::string container_name;
	MyGUI::WidgetPtr widget;
	StringPairs mProperty;
	MapString mUserString;
	// necessary
	std::string type;
	std::string skin;
	std::string position;
	// or
	std::string position_real;
	// necessary for root widgets
	std::string layer;
	// not necessary
	std::string align;
	std::string name;
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
	void add(std::string _name, MyGUI::WidgetPtr _widget);
	void add(WidgetContainer * _container);
private:
	void parseWidget(MyGUI::xml::xmlNodeIterator & _widget, MyGUI::WidgetPtr _parent);
	void serialiseWidget(WidgetContainer * _container, MyGUI::xml::xmlNodePtr _node);

	std::vector<WidgetContainer*> widgets;
};

#endif // __WIDGET_CONTAINER_H__