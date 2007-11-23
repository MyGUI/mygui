#pragma once

#include "BasisState.h"

class OptionsState : public BasisState
{
public:

	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

public:
	void enter(bool bIsChangeState);
	void exit();
	void windowResize(); // уведомление об изменении размеров окна рендера

};