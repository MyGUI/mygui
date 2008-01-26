#include "WidgetContainer.h"

INSTANCE_IMPLEMENT(EditorWidgets);

void EditorWidgets::load()
{
}

void EditorWidgets::save()
{
}

void EditorWidgets::add(std::string _name, MyGUI::WidgetPtr _widget, MyGUI::WidgetPtr _widget_rectangle)
{
	widgets.push_back(new WidgetContainer(_name, _widget, _widget_rectangle));
}

WidgetContainer * EditorWidgets::find(MyGUI::WidgetPtr _widget, bool _isRectangle)
{
	// найдем соответствующий виджет и переместим/растянем
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		if (_isRectangle){
			if ((*iter)->widget_rectangle == _widget)
			{
				return *iter;
			}
		}
		else
		{
			if ((*iter)->widget == _widget)
			{
				return *iter;
			}
		}
	}
	MYGUI_EXCEPT("как это не нашли виджет?");
	return null;
}
WidgetContainer * EditorWidgets::find(std::string _name, bool _isRectangle)
{
	// найдем соответствующий виджет и переместим/растянем
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		if (_isRectangle){
			if ((*iter)->name == _name + "_rectangle")
			{
				return *iter;
			}
		}else{
			if ((*iter)->name == _name)
			{
				return *iter;
			}
		}
	}
	MYGUI_EXCEPT("как это не нашли виджет?");
	return null;
}
