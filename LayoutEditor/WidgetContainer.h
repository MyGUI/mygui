#ifndef __WIDGET_CONTAINER_H__
#define __WIDGET_CONTAINER_H__

#include <vector>
#include <MyGUI_Widget.h>

class WidgetContainer
{
public:
	WidgetContainer(){}
	WidgetContainer(std::string _name, MyGUI::WidgetPtr _widget, MyGUI::WidgetPtr _widget_rectangle):
		name(_name),
		widget(_widget),
		widget_rectangle(_widget_rectangle)
	{}

public:
	std::string name;
	MyGUI::WidgetPtr widget;
	MyGUI::WidgetPtr widget_rectangle;
};

class EditorWidgets{

	INSTANCE_HEADER(EditorWidgets);

public:
	void initialise();
	void shutdown();
	bool load(std::string _fileName);
	bool save(std::string _fileName);
	WidgetContainer * find(MyGUI::WidgetPtr _widget, bool _isRectangle = 0);
	WidgetContainer * find(std::string _name, bool _isRectangle = 0);
	void add(std::string _name, MyGUI::WidgetPtr _widget, MyGUI::WidgetPtr _widget_rectangle);
private:
	std::vector<WidgetContainer*> widgets;
};

#endif // __WIDGET_CONTAINER_H__