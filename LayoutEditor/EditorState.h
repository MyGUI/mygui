#pragma once

#include "BasisState.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Window.h"
#include "MyGUI_Button.h"
#include "MyGUI_Edit.h"

class EditorState : public BasisState
{
public:

	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

	bool frameStarted(const Ogre::FrameEvent& evt);

	void notifyLoad(MyGUI::WidgetPtr _sender = 0, bool _double = 0);
	void notifySave(MyGUI::WidgetPtr _sender = 0, bool _double = 0);
	void notifySaveAs(MyGUI::WidgetPtr _sender = 0, bool _double = 0);
  void notifyQuit(MyGUI::WidgetPtr _sender = 0, bool _double = 0);

  void notifyWidgetSelect(MyGUI::WidgetPtr _sender, bool _double);

	void notifyNewGridStep(MyGUI::WidgetPtr _sender);

	// info for new widget
	int x1, y1, x2, y2;
	std::string current_widget_type;
	MyGUI::EditPtr current_widget;
	int creating_status;

	// current settings
	int grid_step;
public:
	void enter(bool bIsChangeState);
	void exit();

private:
	size_t grid;
};
