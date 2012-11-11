/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mConsole(nullptr),
		mEdit(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_Console");
		addResourceLocation(getRootMedia() + "/Common/Demos");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		if (root.size() == 1)
			root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Write commands in console to change some widget parameters. For example \"colour 1 0 0 1\" changes text colour to red.");

		mEdit = MyGUI::Gui::getInstance().createWidget<MyGUI::EditBox>("EditBoxStretch", MyGUI::IntCoord(10, 80, 100, 100), MyGUI::Align::Default, "Overlapped");
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
		mConsole->unregisterConsoleDelegate("colour");
		mConsole->unregisterConsoleDelegate("show");
		mConsole->unregisterConsoleDelegate("alpha");
		mConsole->unregisterConsoleDelegate("coord");

		delete mConsole;
		mConsole = nullptr;
	}

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (_key == MyGUI::KeyCode::Grave)
		{
			mConsole->setVisible(!mConsole->getVisible());
			return;
		}

		base::BaseManager::injectKeyPress(_key, _text);
	}

	void DemoKeeper::command(const MyGUI::UString& _key, const MyGUI::UString& _value)
	{
		if (_key == "colour")
		{
			if (_value.empty()) mConsole->addToConsole(mConsole->getConsoleStringCurrent(), _key, MyGUI::utility::toString(mEdit->getTextColour()));
			else
			{
				MyGUI::Colour colour;
				if ( ! MyGUI::utility::parseComplex(_value, colour.red, colour.green, colour.blue, colour.alpha))
				{
					mConsole->addToConsole(mConsole->getConsoleStringError(), _key, _value);
					mConsole->addToConsole(mConsole->getConsoleStringFormat(), _key, "red green blue alpha");
				}
				else
				{
					mConsole->addToConsole(mConsole->getConsoleStringSuccess(), _key, _value);
					mEdit->setTextColour(colour);
				}
			}
		}
		else if (_key == "show")
		{
			if (_value.empty())
			{
				mConsole->addToConsole(mConsole->getConsoleStringCurrent(), _key, MyGUI::utility::toString(mEdit->getVisible()));
			}
			else
			{
				bool show = false;
				if ( ! MyGUI::utility::parseComplex(_value, show))
				{
					mConsole->addToConsole(mConsole->getConsoleStringError(), _key, _value);
					mConsole->addToConsole(mConsole->getConsoleStringFormat(), _key, "true | false");
				}
				else
				{
					mConsole->addToConsole(mConsole->getConsoleStringSuccess(), _key, _value);
					mEdit->setVisible(show);
				}
			}
		}
		else if (_key == "alpha")
		{
			if (_value.empty())
			{
				mConsole->addToConsole(mConsole->getConsoleStringCurrent(), _key, MyGUI::utility::toString(mEdit->getAlpha()));
			}
			else
			{
				float alpha;
				if ( ! MyGUI::utility::parseComplex(_value, alpha) || (alpha < 0 || alpha > 1))
				{
					mConsole->addToConsole(mConsole->getConsoleStringError(), _key, _value);
					mConsole->addToConsole(mConsole->getConsoleStringFormat(), _key, "0 - 1");
				}
				else
				{
					mConsole->addToConsole(mConsole->getConsoleStringSuccess(), _key, _value);
					mEdit->setAlpha(alpha);
				}
			}
		}
		else if (_key == "coord")
		{
			if (_value.empty()) mConsole->addToConsole(mConsole->getConsoleStringCurrent(), _key, MyGUI::utility::toString(mEdit->getCoord()));
			else
			{
				MyGUI::IntCoord coord;
				if ( ! MyGUI::utility::parseComplex(_value, coord.left, coord.top, coord.width, coord.height))
				{
					mConsole->addToConsole(mConsole->getConsoleStringError(), _key, _value);
					mConsole->addToConsole(mConsole->getConsoleStringFormat(), _key, "left top width height");
				}
				else
				{
					mConsole->addToConsole(mConsole->getConsoleStringSuccess(), _key, _value);
					mEdit->setCoord(coord);
				}
			}
		}

	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
