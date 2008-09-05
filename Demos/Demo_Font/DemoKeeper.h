/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include <MyGUI.h>
#include "FontPanel.h"

class DemoKeeper
{

public:
	void start();
	void end();

private:
	FontPanel mFontPanel;
};

#endif // __DEMO_KEEPER_H__
