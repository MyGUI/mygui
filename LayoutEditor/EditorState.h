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
	void notifySave(MyGUI::WidgetPtr _sender, bool _double = 0);
	void notifyLoadSaveAs(MyGUI::WidgetPtr _sender, bool _double = 0);
  void notifyQuit(MyGUI::WidgetPtr _sender, bool _double = 0);

	void notifyLoadSaveAccept(MyGUI::WidgetPtr _sender, bool _double = 0);
	// дублирует предыдущий метод
	void notifyLoadSaveEditAccept(MyGUI::WidgetPtr _widget = 0);
	void notifyLoadSaveCancel(MyGUI::WidgetPtr _sender, bool _double = 0);

	// widget panel
  void notifySelectWidgetType(MyGUI::WidgetPtr _sender, bool _double = 0);

	// settings panel
	void notifyNewGridStep(MyGUI::WidgetPtr _sender = 0, MyGUI::WidgetPtr _new = 0);
	// дублирует предыдущий метод
	void notifyNewGridStepAccept(MyGUI::WidgetPtr _sender = 0);

	// widget selecting/resizing
	void notifySelectWidget(MyGUI::WidgetPtr _sender, bool _double = 0);
	void notifyRectangleResize(MyGUI::WidgetPtr _sender);

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
};
