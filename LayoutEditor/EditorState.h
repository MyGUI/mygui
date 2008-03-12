#pragma once

#include "BasisState.h"
#include "MyGUI.h"

#define NO_EXCLUSIVE_INPUT

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

	void windowResize();
private:
	void loadSettings();
	void saveSettings();
	// main panel
	void notifySave(MyGUI::WidgetPtr _sender);
	void notifyLoadSaveAs(MyGUI::WidgetPtr _sender);
	void notifySettings(MyGUI::WidgetPtr _sender);
	void notifyTest(MyGUI::WidgetPtr _sender = 0);
	void notifyClear(MyGUI::WidgetPtr _sender = 0);
  void notifyQuit(MyGUI::WidgetPtr _sender = 0);

	void notifyClearMessage(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button);
	void clear();
	void notifyQuitMessage(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button);

	// save load message
	void notifyLoadSaveAccept(MyGUI::WidgetPtr _sender);
	void notifyLoadSaveEditAccept(MyGUI::WidgetPtr _widget = 0); // calls previous method
	void notifyLoadSaveCancel(MyGUI::WidgetPtr _sender = 0);

	// widget panel
  void notifySelectWidgetType(MyGUI::WidgetPtr _sender);
	void notifySelectWidgetTypeDoubleclick(MyGUI::WidgetPtr _sender);
	void notifyWidgetsTabPressed(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
	void notifyWidgetsTabSelect(MyGUI::WidgetPtr _widget, size_t _index);

	// settings panel
	void notifyNewGridStep(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new = 0);
	void notifyNewGridStepAccept(MyGUI::WidgetPtr _sender); // calls previous method
	void notifyOkSettings(MyGUI::WidgetPtr _sender);

	// widget selecting, properties panel
	void notifySelectWidget(MyGUI::WidgetPtr _sender);

	void updatePropertiesPanel(MyGUI::WidgetPtr _widget);
	void createPropertiesWidgetsPair(MyGUI::WindowPtr _window, Ogre::String _property, Ogre::String _value, std::string _type, int x1, int x2, int w1, int w2 ,int y, int h);
	void createSeparator(MyGUI::WindowPtr _window, std::string _caption, int x1, int y, int w, int h);
	void notifyApplyProperties(MyGUI::WidgetPtr _sender);
	void notifyApplyPropertiesCombo(MyGUI::WidgetPtr _widget); // calls previous method
	void notifyToggleRelativeMode(MyGUI::WidgetPtr _sender = 0);

	// widget editing
	void notifyRectangleResize(MyGUI::WidgetPtr _sender);
	void notifyRectangleDoubleClick(MyGUI::WidgetPtr _sender);
	void notifyRectangleKeyPressed(MyGUI::WidgetPtr _sender, int _key, MyGUI::Char _char);
	void addSheetToTab(MyGUI::WidgetPtr _tab, Ogre::String _caption = "");

	void notifyMinimizePanel(MyGUI::WidgetPtr _sender);
	// strings panel
	void syncStrings(bool _apply, bool _add = 0, Ogre::String _value = "");
	void notifyAddString(MyGUI::WidgetPtr _sender);
	void notifyDeleteString(MyGUI::WidgetPtr _sender);
	void notifySelectString(MyGUI::WidgetPtr _sender);
	void notifyUpdateString(MyGUI::WidgetPtr _widget);
	void notifySelectStringItem(MyGUI::WidgetPtr _widget, size_t _position);
	// UserData panel

	MyGUI::IntCoord convertCoordToParentCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget); // это можно в методы гуи занести
	MyGUI::IntCoord convertParentCoordToCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget); // это можно в методы гуи занести

	MyGUI::ComboBoxPtr allWidgetsCombo;

	// info for new widget
	int x1, y1, x2, y2;
	std::string current_widget_type;
	std::string current_widget_skin;
	MyGUI::WidgetPtr current_widget;
	MyGUI::WindowPtr current_widget_rectangle;
	// 0 - none, 1 - mouse pressed (prepare), 2 - mouse moved (widget created)
	int creating_status;
	// drop select after skin change
	bool recreate;
	// widget was moved by keyboard arrows
	bool arrow_move;

	// current settings
	int grid_step;

	bool shiftPressed;
	bool ctrlPressed;
	bool want_quit;
	// last loaded/saved file name
	Ogre::String fileName;

	// properties wndow
	MyGUI::VectorWidgetPtr propertiesText;
	MyGUI::VectorWidgetPtr propertiesElement;
};
