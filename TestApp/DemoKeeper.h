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

	void command(MyGUI::WidgetPtr _sender, const Ogre::UTFString & _key, const Ogre::UTFString & _value);

	void consolePressed();
	void eventToolTip(MyGUI::WidgetPtr _sender, MyGUI::ToolTipInfo _info);

private:
	MyGUI::Gui * mGUI;
	size_t mWidth, mHeight;
	MyGUI::ConsolePtr mConsole;

};

#endif // __DEMO_KEEPER_H__
