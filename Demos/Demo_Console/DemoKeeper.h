/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include <MyGUI.h>
#include "Console.h"

class DemoKeeper
{

public:
	void start();
	void end();

	void pressConsoleKey();

private:
	void command(const Ogre::UTFString & _key, const Ogre::UTFString & _value);

private:
	Console mConsole;
};

#endif // __DEMO_KEEPER_H__
