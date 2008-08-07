/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "DemoKeeper.h"
#include "BasisManager.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

DemoKeeper::DemoKeeper() :
	mGUI(0),
	mWidth(1), mHeight(1),
	mConsole(0)
{
}

void DemoKeeper::command(MyGUI::WidgetPtr _sender, const Ogre::UTFString & _key, const Ogre::UTFString & _value)
{
	MyGUI::ConsolePtr console = MyGUI::castWidget<MyGUI::Console>(_sender);

	static Ogre::ColourValue colour;
	static bool show = false;

	if (_key == "colour") {
		if (_value.empty()) console->addToConsole(console->getConsoleStringCurrent(), _key, MyGUI::utility::toString(colour));
		else {
			if ( ! MyGUI::utility::parseComplex(_value, colour.r, colour.g, colour.b, colour.a)) {
				console->addToConsole(console->getConsoleStringError(), _key, _value);
				console->printCommandFormat(_key);
			}
			else console->addToConsole(console->getConsoleStringSuccess(), _key, _value);
		}
	}
	else if (_key == "show") {
		if (_value.empty()) console->addToConsole(console->getConsoleStringCurrent(), _key, MyGUI::utility::toString(show));
		else {
			if ( ! MyGUI::utility::parseComplex(_value, show)) {
				console->addToConsole(console->getConsoleStringError(), _key, _value);
				console->printCommandFormat(_key);
			}
			else console->addToConsole(console->getConsoleStringSuccess(), _key, _value);
		}
	}

}

void DemoKeeper::start(MyGUI::Gui * _gui, size_t _width, size_t _height)
{
    mGUI = _gui;
    mWidth = _width;
    mHeight = _height;

	/*MyGUI::SkinManager::getInstance().load("WOT_new.skin");
	MyGUI::FontManager::getInstance().load("WOT.font");
	MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WOT_WindowCharacter", MyGUI::IntCoord(100, 100, 500, 500), MyGUI::ALIGN_DEFAULT, "Main");
	window->setCaption("test");*/

	MyGUI::LanguageManager::getInstance().setCurrentLanguage("rus");
	Ogre::UTFString str = MyGUI::LanguageManager::getInstance().replaceLanguage(L"я хачу #{Blond}, но я не #{Blond}!");
	MyGUI::Message::createMessage("none", str, false, MyGUI::Message::None);

	//MyGUI::FontManager::getInstance().saveFontTexture("MyGUI_CoreFont.18", "MyGUI_CoreFont.18.png");

	//mConsole = mGUI->createWidget<MyGUI::Console>("Console", MyGUI::IntCoord(100, 100, 540, 226), MyGUI::ALIGN_DEFAULT, "Main");

	//mConsole->registerConsoleDelegate("colour", newDelegate(this, &DemoKeeper::command), "colour red green blue alpha");
	//mConsole->registerConsoleDelegate("show", newDelegate(this, &DemoKeeper::command), "show true | false");

	/*std::string read, text;
	std::ifstream stream("eula.txt");
	if (false != stream.is_open()) {
		while (false == stream.eof()) {
			// берем новую строку

			std::getline(stream, read);
			text += read + "\n";
		};
	}

	MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(100, 100, 500, 500), MyGUI::ALIGN_DEFAULT, "Main");
	MyGUI::EditPtr edit = window->createWidget<MyGUI::Edit>("EditStretch", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::ALIGN_STRETCH);
	//edit->showHScroll(false);
	//edit->setTextAlign(MyGUI::ALIGN_DEFAULT);
	edit->setEditMultiLine(true);
	edit->setEditStatic(true);
	edit->setEditBreakLine(true);
	//edit->setEditReadOnly(true);
	edit->setMaxTextLength(15000);
	edit->setCaption(text);//*/

	/*MyGUI::Message::createMessage("none", "test uiyuiyuitoiutoiutoiutioutoiutoitiu_\nujgfouigifgalf\npiaufgaiufa[f\nPIUFGIugfiGFGifgIFGiufgIFGJhgfjllgljfakfhak;hfklahflkfjh_", false, MyGUI::Message::None);
	MyGUI::Message::createMessage("icon", "test uiyuiyuitoiutoiutoiutioutoiutoitiu_\nujgfouigifgalf\npiaufgaiufa[f\nPIUFGIugfiGFGifgIFGiufgIFGJhgfjllgljfakfhak;hfklahflkfjh_", false, 0 | MyGUI::Message::IconQuest);
	MyGUI::Message::createMessage("button", "test uiyuiyuitoiutoiutoiutioutoiutoitiu_\nujgfouigifgalf\npiaufgaiufa[f\nPIUFGIugfiGFGifgIFGiufgIFGJhgfjllgljfakfhak;hfklahflkfjh_", false, 0 | MyGUI::Message::Ok);*/
	/*edit->setTextAlign(MyGUI::ALIGN_HCENTER | MyGUI::ALIGN_VCENTER);
	edit->setCaption("ComplexProgress");//*/
	

	//MyGUI::RenderBoxPtr box = mGUI->createWidget<MyGUI::RenderBox>("RenderBox", MyGUI::IntCoord(100, 100, 600, 600), MyGUI::ALIGN_DEFAULT, "Main");
	//box->injectObject("robot.mesh");
	//box->setMouseRotation(true);
	//box->setViewScale(true);
	//box->setAutoRotation(true);
	//box->setAutoRotation(true);

	/*MyGUI::StaticImagePtr image = mGUI->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(100, 100, 42, 75), MyGUI::ALIGN_DEFAULT, "Main");
	image->setImageTexture("child.png");
	image->addItem(MyGUI::IntCoord(0, 0, 42, 75));
	image->addItemFrame(0, MyGUI::IntCoord(42, 0, 42, 75));
	image->addItemFrame(0, MyGUI::IntCoord(42*2, 0, 42, 75));
	image->addItemFrame(0, MyGUI::IntCoord(42*3, 0, 42, 75));
	image->addItemFrameDublicate(0, 0);
	image->addItemFrameDublicate(0, 3);
	image->setItemFrameRate(0, 0.15);
	image->setItemSelect(0);*/

	/*MyGUI::WidgetPtr widget = mGUI->createWidget<MyGUI::Widget>("ButtonSmall", MyGUI::IntCoord(100, 100, 42, 75), MyGUI::ALIGN_DEFAULT, "Main");
	widget->setToolTipEnable(true);
	widget->eventToolTip = newDelegate(this, &DemoKeeper::eventToolTip);*/

	/*MyGUI::EditPtr edit = mGUI->createWidget<MyGUI::Edit>("EditStretch", MyGUI::IntCoord(100, 100, 340, 26), MyGUI::ALIGN_DEFAULT, "Main");
	edit->setTextAlign(MyGUI::ALIGN_HCENTER | MyGUI::ALIGN_VCENTER);
	edit->setCaption("ComplexProgress");//*/

	/*MyGUI::ButtonPtr button = mGUI->createWidget<MyGUI::Button>("ButtonSmall", MyGUI::IntCoord(100, 140, 100, 26), MyGUI::ALIGN_DEFAULT, "Main");
	button->setTextAlign(MyGUI::ALIGN_HCENTER | MyGUI::ALIGN_VCENTER);
	button->setCaption("ComplexProgress");//*/

   /*MyGUI::MultiListPtr mlist = mGUI->createWidget<MyGUI::MultiList>("MultiList", MyGUI::IntCoord(100, 100, 340, 340), MyGUI::ALIGN_DEFAULT, "Main");
   mlist->addColumn(100, "columnA");
	mlist->addColumn(100, "columnB");
	mlist->addItem("1");
	mlist->addItem("3");
	mlist->addItem("2");
	mlist->addItem("4");
   mlist->setSubItem(1,0,"d");
	mlist->setSubItem(1,1,"b");
	mlist->setSubItem(1,2,"a");
	mlist->setSubItem(1,3,"c");*/

	/*MyGUI::PopupMenuPtr menu = mGUI->createWidget<MyGUI::PopupMenu>("PopupMenu", MyGUI::IntCoord(10, 10, 100, 100), MyGUI::ALIGN_DEFAULT, "Popup");
	menu->addItem("test");
	menu->showPopupMenu(MyGUI::IntPoint(100, 100));*/

}

void DemoKeeper::end()
{
}

void DemoKeeper::consolePressed()
{
	if (mConsole) {
		mConsole->showConsole( ! mConsole->isShowConsole());
	}
}

void DemoKeeper::eventToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
{
	if (_info.type == MyGUI::TOOLTIP_SHOW) {
		MyGUI::MYGUI_OUT("SHOW index : ", _info.index);
	}
	else if (_info.type == MyGUI::TOOLTIP_HIDE) {
		MyGUI::MYGUI_OUT("HIDE");
	}
}
