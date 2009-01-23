#ifndef __EDITOR_WIDGETS_H__
#define __EDITOR_WIDGETS_H__

#include <sstream>
#include "WidgetContainer.h"

void MapSet(StringPairs & _map, const std::string &_key, const std::string &_value);
StringPairs::iterator MapFind(StringPairs & _map, const std::string &_key);
void MapErase(StringPairs & _map, const std::string &_key);

MyGUI::IntCoord convertCoordToParentCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget); // это можно в методы гуи занести

class EditorWidgets{

	MYGUI_INSTANCE_HEADER(EditorWidgets);

public:
	void initialise();
	void shutdown();
	bool load(std::string _fileName);
	bool save(std::string _fileName);
	void loadxmlDocument(MyGUI::xml::Document * doc, bool _test = false);
	MyGUI::xml::Document * savexmlDocument();
	WidgetContainer * find(MyGUI::WidgetPtr _widget);
	WidgetContainer * find(std::string _name);
	void add(WidgetContainer * _container);
	void remove(MyGUI::WidgetPtr _widget);
	void remove(WidgetContainer * _container);
	void clear();

	bool tryToApplyProperty(MyGUI::WidgetPtr _widget, std::string _key, std::string _value, bool _test = false);

	std::vector<WidgetContainer*> widgets;
	int global_counter;
	bool widgets_changed;
private:
	WidgetContainer * _find(MyGUI::WidgetPtr _widget, std::string _name, std::vector<WidgetContainer*> _widgets);

	void parseWidget(MyGUI::xml::ElementEnumerator & _widget, MyGUI::WidgetPtr _parent, bool _test = false);
	void serialiseWidget(WidgetContainer * _container, MyGUI::xml::ElementPtr _node);

	void loadIgnoreParameters(MyGUI::xml::ElementPtr _node, const std::string & _file, MyGUI::Version _version);

	std::vector<std::string> ignore_parameters;
};

#endif // __EDITOR_WIDGETS_H__
