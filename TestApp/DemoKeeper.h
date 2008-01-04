/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "MyGUI_Gui.h"

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
	void notifyListPressedDelete(MyGUI::WidgetPtr _sender);
	void notifyWindowXPressed(MyGUI::WidgetPtr _widget, const std::string& _name);

	void notifyWindowButton1(MyGUI::WidgetPtr _sender, bool _double);
	void notifyWindowButton2(MyGUI::WidgetPtr _sender, bool _double);

	void createWindowEdit();
	void createWindowList();

private:
	MyGUI::Gui * mGUI;
	size_t mCountWindow;
	size_t mWidth, mHeight;

};

#endif // __DEMO_KEEPER_H__
