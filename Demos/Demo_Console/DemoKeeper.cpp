/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"
#include "BasisManager.h"

void DemoKeeper::start()
{
	// загружаем ресурсы для демо
	// потом сделать и для мака
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../Media/Demos/Demo_Console", "FileSystem", "General");

	mConsole.initialise();

	mConsole.registerConsoleDelegate("colour", MyGUI::newDelegate(this, &DemoKeeper::command));//, "colour red green blue alpha");
	mConsole.registerConsoleDelegate("show", MyGUI::newDelegate(this, &DemoKeeper::command));//, "show true | false");
}

void DemoKeeper::end()
{
	mConsole.shutdown();
}

void DemoKeeper::pressConsoleKey()
{
	mConsole.isShow() ? mConsole.hide() : mConsole.show();
}

void DemoKeeper::command(const Ogre::UTFString & _key, const Ogre::UTFString & _value)
{
	static Ogre::ColourValue colour;
	static bool show = false;

	if (_key == "colour") {
		if (_value.empty()) mConsole.addToConsole(mConsole.getConsoleStringCurrent(), _key, MyGUI::utility::toString(colour));
		else {
			if ( ! MyGUI::utility::parseComplex(_value, colour.r, colour.g, colour.b, colour.a)) {
				mConsole.addToConsole(mConsole.getConsoleStringError(), _key, _value);
				mConsole.addToConsole(mConsole.getConsoleStringFormat(), _key, "red green blue alpha");
			}
			else {
				mConsole.addToConsole(mConsole.getConsoleStringSuccess(), _key, _value);
			}
		}
	}
	else if (_key == "show") {
		if (_value.empty()) {
			mConsole.addToConsole(mConsole.getConsoleStringCurrent(), _key, MyGUI::utility::toString(show));
		}
		else {
			if ( ! MyGUI::utility::parseComplex(_value, show)) {
				mConsole.addToConsole(mConsole.getConsoleStringError(), _key, _value);
				mConsole.addToConsole(mConsole.getConsoleStringFormat(), _key, "true | false");
			}
			else {
				mConsole.addToConsole(mConsole.getConsoleStringSuccess(), _key, _value);
			}
		}
	}

}
