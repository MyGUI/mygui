/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include <MyGUI.h>

class DemoKeeper
{

public:
	void start();
	void end();

private:
	MyGUI::WidgetPtr m_button;

};

#endif // __DEMO_KEEPER_H__
