#pragma once

#include "BasisState.h"

class OptionsState : public BasisState
{
public:

	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

public:
	void enter(bool bIsChangeState);
	void exit();
	void windowResize(); // уведомление об изменении размеров окна рендера

};