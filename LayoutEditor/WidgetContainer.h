#ifndef __WIDGET_CONTAINER_H__
#define __WIDGET_CONTAINER_H__

#include "MyGUI.h"

typedef std::vector<std::pair<Ogre::String, Ogre::String> > StringPairs;
typedef std::map<Ogre::String, Ogre::String> MapString;

struct WidgetContainer
{
	WidgetContainer(){}
	WidgetContainer(std::string _type, std::string _skin, MyGUI::WidgetPtr _widget, std::string _name = ""):
		type(_type),
		skin(_skin),
		widget(_widget),
		name(_name)
	{}

	MyGUI::WidgetPtr widget;
	StringPairs mProperty;
	MapString mUserString;
	std::string type;	// necessary
	std::string skin;	// necessary (except Sheet)
	std::string position; // necessary this or position_real (except Sheet)
	std::string position_real; // necessary this or position (except Sheet)
	std::string layer; // necessary for root widgets
	std::string align; // not necessary
	Ogre::String name; // not necessary
};

class EditorWidgets{

	INSTANCE_HEADER(EditorWidgets);

public:
	void initialise();
	void shutdown();
	bool load(std::string _fileName);
	bool save(std::string _fileName);
	void loadxmlDocument(MyGUI::xml::xmlDocument * doc);
	MyGUI::xml::xmlDocument * savexmlDocument();
	WidgetContainer * find(MyGUI::WidgetPtr _widget);
	WidgetContainer * find(std::string _name);
	void add(WidgetContainer * _container);
	void remove(MyGUI::WidgetPtr _widget);
	void clear();

	std::vector<WidgetContainer*> widgets;
	int global_counter;
private:
	void parseWidget(MyGUI::xml::xmlNodeIterator & _widget, MyGUI::WidgetPtr _parent);
	void serialiseWidget(WidgetContainer * _container, MyGUI::xml::xmlNodePtr _node);
};

#endif // __WIDGET_CONTAINER_H__
