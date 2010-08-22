#ifndef __EDITOR_WIDGETS_H__
#define __EDITOR_WIDGETS_H__

#include <sstream>
#include "WidgetContainer.h"

void MapSet(MyGUI::VectorStringPairs & _map, const std::string &_key, const std::string &_value);
MyGUI::VectorStringPairs::iterator MapFind(MyGUI::VectorStringPairs & _map, const std::string &_key);
void MapErase(MyGUI::VectorStringPairs & _map, const std::string &_key);

// это можно в методы гуи занести
MyGUI::IntCoord convertCoordToParentCoord(const MyGUI::IntCoord& _coord, MyGUI::Widget* _widget);

class CodeGenerator;

class EditorWidgets :
	public MyGUI::Singleton<EditorWidgets>
{
public:
	EditorWidgets();
	~EditorWidgets();

public:
	void initialise();
	void shutdown();
	bool load(const MyGUI::UString& _fileName);
	bool save(const MyGUI::UString& _fileName);
	void loadxmlDocument(MyGUI::xml::Document * doc, bool _test = false);
	MyGUI::xml::Document * savexmlDocument();
	WidgetContainer * find(MyGUI::Widget* _widget);
	WidgetContainer * find(const std::string& _name);
	void add(WidgetContainer * _container);
	void remove(MyGUI::Widget* _widget);
	void remove(WidgetContainer * _container);
	void clear();

	void setCodeGenerator(CodeGenerator* _codeGenerator) { mCodeGenerator = _codeGenerator; }

	bool tryToApplyProperty(MyGUI::Widget* _widget, const std::string& _key, const std::string& _value, bool _test = false);

	void invalidateWidgets();

	std::vector<WidgetContainer*> widgets;
	int global_counter;

private:
	WidgetContainer * _find(MyGUI::Widget* _widget, const std::string& _name, std::vector<WidgetContainer*> _widgets);

	void parseWidget(MyGUI::xml::ElementEnumerator & _widget, MyGUI::Widget* _parent, bool _test = false);
	void serialiseWidget(WidgetContainer * _container, MyGUI::xml::ElementPtr _node);

	void loadIgnoreParameters(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);
	void notifyFrameStarted(float _time);

private:
	bool widgets_changed;
	std::vector<std::string> ignore_parameters;
	CodeGenerator* mCodeGenerator;
};

#endif // __EDITOR_WIDGETS_H__
