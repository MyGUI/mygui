#ifndef __WIDGET_CONTAINER_H__
#define __WIDGET_CONTAINER_H__

#include "MyGUI.h"

typedef std::vector<std::pair<Ogre::String, Ogre::String> > StringPairs;

const std::string DEFAULT_EDITOR_LAYER = "LayoutEditor_Widgets";
const std::string DEFAULT_LAYER = "Back";

MyGUI::IntCoord convertCoordToParentCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget); // ýòî ìîæíî â ìåòîäû ãóè çàíåñòè

struct WidgetContainer
{
	WidgetContainer():
		relative_mode(false)
	{}
	WidgetContainer(std::string _type, std::string _skin, MyGUI::WidgetPtr _widget, std::string _name = ""):
		widget(_widget),
		type(_type),
		skin(_skin),
		name(_name),
		layer(DEFAULT_LAYER),
		relative_mode(false)
	{
	}

	MyGUI::WidgetPtr widget;
	std::vector<WidgetContainer*> childContainers;
	StringPairs mProperty;
	StringPairs mUserString;
	std::string type;
	std::string skin;
	std::string align;
	Ogre::String name;
	Ogre::String layer;
	// not for saving
	std::string position(bool _percent = true)
	{
		if (relative_mode){
			MyGUI::FloatCoord coord = MyGUI::WidgetManager::getInstance().convertIntToRelative(widget->getCoord(), static_cast<MyGUI::WidgetPtr>(widget->getCroppedParent()));
			std::ostringstream stream;
			if (_percent) stream << coord.left*100 << " " << coord.top*100 << " " << coord.width*100 << " " << coord.height*100;
			else stream << coord.left << " " << coord.top << " " << coord.width << " " << coord.height;
			return stream.str();
		}
		return widget->getCoord().print();
	}
	bool relative_mode;
};

#endif // __WIDGET_CONTAINER_H__
