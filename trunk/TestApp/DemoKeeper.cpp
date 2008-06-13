/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "DemoKeeper.h"
#include "BasisManager.h"

DemoKeeper::DemoKeeper() :
	mGUI(0),
	mWidth(1), mHeight(1)
{
}

void DemoKeeper::start(MyGUI::Gui * _gui, size_t _width, size_t _height)
{
    mGUI = _gui;
    mWidth = _width;
    mHeight = _height;

	//MyGUI::FontManager::getInstance().saveFontTexture("Microsoft_Sans_Serif.13", "Microsoft_Sans_Serif.13.png");

	//MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(100, 100, 300, 300), MyGUI::ALIGN_DEFAULT, "Main");
	MyGUI::EditPtr edit = mGUI->createWidget<MyGUI::Edit>("EditStretch", MyGUI::IntCoord(100, 100, 421, 480), MyGUI::ALIGN_STRETCH, "Main");
	edit->setTextAlign(MyGUI::ALIGN_CENTER);
	edit->setEditMultiLine(true);
	edit->setCaption("1\t2 3");

}

void DemoKeeper::end()
{
}
