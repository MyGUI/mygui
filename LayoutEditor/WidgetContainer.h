#ifndef __WIDGET_CONTAINER_H__
#define __WIDGET_CONTAINER_H__

#include <vector>
#include "MyGUI.h"

class WidgetContainer : public MyGUI::UserData
{
public:
	WidgetContainer(){}
	WidgetContainer(std::string _name, MyGUI::WidgetPtr _widget):
		name(_name),
		widget(_widget)
	{}

public:
	MyGUI::WidgetPtr widget;
	std::string name;
	std::string type;
	std::string skin;
	std::string layer;
	std::string align;
	std::string position;
	std::string position_real;
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

	std::vector<WidgetContainer*> widgets;
};

#endif // __WIDGET_CONTAINER_H__