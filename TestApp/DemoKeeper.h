/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "MyGUI.h"

class DemoKeeper
{
public:
	DemoKeeper();

	void start(MyGUI::Gui * _gui, size_t _width, size_t _height);
	void end();

private:
	void notifyPressedReadOnly(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedPassword(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedMultiLine(MyGUI::WidgetPtr _sender, bool _double);

	void notifyPressedColourGreen(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedColourRed(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedColourBlue(MyGUI::WidgetPtr _sender, bool _double);

	void notifyPressedAdd(MyGUI::WidgetPtr _sender, bool _double);
	void notifyPressedDelete(MyGUI::WidgetPtr _sender, bool _double);

	void notifyEditAccept(MyGUI::WidgetPtr _sender);
	void notifyListButtonPressed(MyGUI::WidgetPtr _sender, int _key, MyGUI::Char _char);
	void notifyWindowXPressed(MyGUI::WidgetPtr _widget, const std::string& _name);

	void notifyWindowButton1(MyGUI::WidgetPtr _sender, bool _double);
	void notifyWindowButton2(MyGUI::WidgetPtr _sender, bool _double);

	void notifyTest(MyGUI::WidgetPtr _sender, size_t _index);

	void createWindowEdit(MyGUI::WidgetPtr _widget);
	void createWindowList(MyGUI::WidgetPtr _widget);

	void test(MyGUI::WidgetPtr _sender, MyGUI::Message::ButtonInfo _button);

private:
	MyGUI::Gui * mGUI;
	size_t mCountWindow;
	size_t mWidth, mHeight;

};

#endif // __DEMO_KEEPER_H__
