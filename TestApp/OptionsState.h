#pragma once

#include "BasisState.h"
#include "MyGUI_Gui.h"

class OptionsState : public BasisState
{
public:

	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

	void notifyPressedReadOnly(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedPassword(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedMultiLine(MyGUI::WidgetPtr _sender, bool _double);

	void notifyPressedColourGreen(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedColourRed(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedColourBlue(MyGUI::WidgetPtr _sender, bool _double);

	void createWindowEdit();

public:
	void enter(bool bIsChangeState);
	void exit();
	void windowResize(); // уведомление об изменении размеров окна рендера

};