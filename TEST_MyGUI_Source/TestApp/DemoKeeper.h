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
	void move(int _left, int _top);

	void test(MyGUI::WidgetPtr _sender, bool _left);

private:
	MyGUI::Gui * mGUI;
	size_t mWidth, mHeight;

};

#endif // __DEMO_KEEPER_H__
