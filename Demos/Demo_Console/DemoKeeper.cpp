/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{
		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Console");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		base::BaseManager::getInstance().setDescriptionText("Write commands in console to change some widget parameters. For example \"colour 1 0 0 1\" changes text colour to red.");

		mEdit = mGUI->createWidget<MyGUI::Edit>("EditStretch", MyGUI::IntCoord(10, 80, 100, 100), MyGUI::Align::Default, "Overlapped");
		mEdit->setCaption("some edit");
		mEdit->setTextAlign(MyGUI::Align::Center);
		mEdit->setEditMultiLine(true);

		mConsole = new Console();

		mConsole->registerConsoleDelegate("colour", MyGUI::newDelegate(this, &DemoKeeper::command));
		mConsole->registerConsoleDelegate("show", MyGUI::newDelegate(this, &DemoKeeper::command));
		mConsole->registerConsoleDelegate("alpha", MyGUI::newDelegate(this, &DemoKeeper::command));
		mConsole->registerConsoleDelegate("coord", MyGUI::newDelegate(this, &DemoKeeper::command));

		mConsole->setVisible(true);
	}

	void DemoKeeper::destroyScene()
	{
		delete mConsole;
	}

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		if ( arg.key == OIS::KC_GRAVE )
		{
			mConsole->setVisible( ! mConsole->isVisible() );
			return true;
		}

		return BaseManager::keyPressed(arg);
	}

	void DemoKeeper::command(const Ogre::UTFString & _key, const Ogre::UTFString & _value)
	{
		if (_key == "colour") {
			if (_value.empty()) mConsole->addToConsole(mConsole->getConsoleStringCurrent(), _key, MyGUI::utility::toString(mEdit->getTextColour()));
			else {
				MyGUI::Colour colour;
				if ( ! MyGUI::utility::parseComplex(_value, colour.red, colour.green, colour.blue, colour.alpha)) {
					mConsole->addToConsole(mConsole->getConsoleStringError(), _key, _value);
					mConsole->addToConsole(mConsole->getConsoleStringFormat(), _key, "red green blue alpha");
				}
				else {
					mConsole->addToConsole(mConsole->getConsoleStringSuccess(), _key, _value);
					mEdit->setTextColour(colour);
				}
			}
		}
		else if (_key == "show") {
			if (_value.empty()) {
				mConsole->addToConsole(mConsole->getConsoleStringCurrent(), _key, MyGUI::utility::toString(mEdit->isVisible()));
			}
			else {
				bool show;
				if ( ! MyGUI::utility::parseComplex(_value, show)) {
					mConsole->addToConsole(mConsole->getConsoleStringError(), _key, _value);
					mConsole->addToConsole(mConsole->getConsoleStringFormat(), _key, "true | false");
				}
				else {
					mConsole->addToConsole(mConsole->getConsoleStringSuccess(), _key, _value);
					mEdit->setVisible(show);
				}
			}
		}
		else if (_key == "alpha") {
			if (_value.empty()) {
				mConsole->addToConsole(mConsole->getConsoleStringCurrent(), _key, MyGUI::utility::toString(mEdit->getAlpha()));
			}
			else {
				float alpha;
				if ( ! MyGUI::utility::parseComplex(_value, alpha) || (alpha < 0 || alpha > 1)) {
					mConsole->addToConsole(mConsole->getConsoleStringError(), _key, _value);
					mConsole->addToConsole(mConsole->getConsoleStringFormat(), _key, "0 - 1");
				}
				else {
					mConsole->addToConsole(mConsole->getConsoleStringSuccess(), _key, _value);
					mEdit->setAlpha(alpha);
				}
			}
		}
		else if (_key == "coord") {
			if (_value.empty()) mConsole->addToConsole(mConsole->getConsoleStringCurrent(), _key, MyGUI::utility::toString(mEdit->getCoord()));
			else {
				MyGUI::IntCoord coord;
				if ( ! MyGUI::utility::parseComplex(_value, coord.left, coord.top, coord.width, coord.height)) {
					mConsole->addToConsole(mConsole->getConsoleStringError(), _key, _value);
					mConsole->addToConsole(mConsole->getConsoleStringFormat(), _key, "left top width height");
				}
				else {
					mConsole->addToConsole(mConsole->getConsoleStringSuccess(), _key, _value);
					mEdit->setCoord(coord);
				}
			}
		}

	}

} // namespace demo
