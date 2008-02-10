#pragma once

#include "BasisState.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Window.h"

class EditorState : public BasisState
{
public:
	void enter(bool bIsChangeState);
	void exit();

	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

	bool frameStarted(const Ogre::FrameEvent& evt);

private:
	void loadSettings();
	void saveSettings();
	// main panel
	void notifySave(MyGUI::WidgetPtr _sender);
	void notifyLoadSaveAs(MyGUI::WidgetPtr _sender);
	void notifySettings(MyGUI::WidgetPtr _sender);
  void notifyQuit(MyGUI::WidgetPtr _sender);

	void notifyLoadSaveAccept(MyGUI::WidgetPtr _sender);
	void notifyLoadSaveEditAccept(MyGUI::WidgetPtr _widget = 0); // calls previous method
	void notifyLoadSaveCancel(MyGUI::WidgetPtr _sender = 0);

	// widget panel
  void notifySelectWidgetType(MyGUI::WidgetPtr _sender);

	// settings panel
	void notifyNewGridStep(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new = 0);
	void notifyNewGridStepAccept(MyGUI::WidgetPtr _sender); // calls previous method
	void notifyOkSettings(MyGUI::WidgetPtr _sender);

	// widget selecting/resizing
	void notifySelectWidget(MyGUI::WidgetPtr _sender);
	void updatePropertiesPanel(MyGUI::WidgetPtr _widget);
	void createPropertiesWidgetsPair(MyGUI::WindowPtr _window, std::string _property, std::string _value, std::string _type, int x1, int x2, int w1, int w2 ,int y, int h);
	void notifyApplyProperties(MyGUI::WidgetPtr _sender);
	void notifyApplyPropertiesCombo(MyGUI::WidgetPtr _widget/*, size_t _index = 0*/); // calls previous method
	void notifyRectangleResize(MyGUI::WidgetPtr _sender);
	void notifyRectangleDoubleClick(MyGUI::WidgetPtr _sender);

	MyGUI::IntCoord convertCoordToParentCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget); // это можно в методы гуи занести
	MyGUI::IntCoord convertParentCoordToCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget); // это можно в методы гуи занести
	MyGUI::IntCoord snapToGrid(MyGUI::IntCoord coord);

	// info for new widget
	int x1, y1, x2, y2;
	std::string current_widget_type;
	std::string current_widget_skin;
	MyGUI::WidgetPtr current_widget;
	MyGUI::WindowPtr current_widget_rectangle;
	// 0 - none, 1 - mouse pressed (prepare), 2 - mouse moved (widget created)
	int creating_status;
	// for unique names
	int counter;

	// current settings
	int grid_step;
	// last loaded/saved file name
	std::string fileName;

	size_t grid;

	// properties wndow
	MyGUI::VectorWidgetPtr propertiesText;
	MyGUI::VectorWidgetPtr propertiesElement;
};
