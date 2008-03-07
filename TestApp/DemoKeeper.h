/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "MyGUI.h"

extern Ogre::Camera* mCamera;

class DemoKeeper
{
public:
	DemoKeeper();

	void start(MyGUI::Gui * _gui, size_t _width, size_t _height);
	void end();

	void press(int _left, int _top);

private:
	void notifyPressedReadOnly(MyGUI::WidgetPtr _sender);
	void notifyPressedPassword(MyGUI::WidgetPtr _sender);
	void notifyPressedMultiLine(MyGUI::WidgetPtr _sender);

	void notifyPressedColourGreen(MyGUI::WidgetPtr _sender);
	void notifyPressedColourRed(MyGUI::WidgetPtr _sender);
	void notifyPressedColourBlue(MyGUI::WidgetPtr _sender);

	void notifyPressedAdd(MyGUI::WidgetPtr _sender);
	void notifyPressedDelete(MyGUI::WidgetPtr _sender);

	void notifyEditAccept(MyGUI::WidgetPtr _sender);
	void notifyListButtonPressed(MyGUI::WidgetPtr _sender, int _key, MyGUI::Char _char);
	void notifyWindowXPressed(MyGUI::WidgetPtr _widget, const std::string& _name);

	void notifyWindowButton1(MyGUI::WidgetPtr _sender);
	void notifyWindowButton2(MyGUI::WidgetPtr _sender);

	void createWindowEdit(MyGUI::WidgetPtr _widget);
	void createWindowList(MyGUI::WidgetPtr _widget);

	void notifyRadio(MyGUI::WidgetPtr _sender);

private:
	MyGUI::Gui * mGUI;
	size_t mCountWindow;
	size_t mWidth, mHeight;

};

#endif // __DEMO_KEEPER_H__
