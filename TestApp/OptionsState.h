#pragma once

#include "BasisState.h"
#include "Gui.h"

class OptionsState : public BasisState
{
public:

	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

	void notifyMousePressed1(MyGUI::WidgetPtr _sender, bool _left);
	void notifyMousePressed2(MyGUI::WidgetPtr _sender, bool _left);

	void notifyWindowXPressed(MyGUI::WidgetPtr _sender);

	MyGUI::WidgetPtr test;

public:
	void enter(bool bIsChangeState);
	void exit();
	void windowResize(); // уведомление об изменении размеров окна рендера

};