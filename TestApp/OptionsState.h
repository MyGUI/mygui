#pragma once

#include "BasisState.h"
#include "MyGUI_Gui.h"
#include "MyGUI_StaticText.h"

class OptionsState : public BasisState
{
public:

	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

	bool frameStarted(const Ogre::FrameEvent& evt);

	void notifyPressedReadOnly(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedPassword(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedMultiLine(MyGUI::WidgetPtr _sender, bool _double);

	void notifyPressedColourGreen(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedColourRed(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedColourBlue(MyGUI::WidgetPtr _sender, bool _double);

	void notifyPressedAdd(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedDelete(MyGUI::WidgetPtr _sender, bool _double);

	void notifyEditAccept(MyGUI::WidgetPtr _sender);
	void notifyListPressedDelete(MyGUI::WidgetPtr _sender);
	void notifyWindowXPressed(MyGUI::WidgetPtr _widget);

	void notifyWindowButton1(MyGUI::WidgetPtr _sender, bool _double);
	void notifyWindowButton2(MyGUI::WidgetPtr _sender, bool _double);

	void createWindowEdit();
	void createWindowList();

	void updateWidgetPosition(Ogre::Entity * _entity, Ogre::Camera * _camera, MyGUI::WidgetPtr _widget);

public:
	void enter(bool bIsChangeState);
	void exit();
	void windowResize(); // уведомление об изменении размеров окна рендера

private:
	MyGUI::StaticTextPtr mFpsInfo;
	size_t mCountWindow;

};