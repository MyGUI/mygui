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

	//MyGUI::FontManager::getInstance().saveFontTexture("MyGUI_CoreFont.18", "MyGUI_CoreFont.18.png");

	//MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(100, 100, 300, 300), MyGUI::ALIGN_DEFAULT, "Main");
	MyGUI::EditPtr edit = mGUI->createWidget<MyGUI::Edit>("EditStretch", MyGUI::IntCoord(100, 100, 121, 80), MyGUI::ALIGN_STRETCH, "Main");
	edit->setTextAlign(MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	edit->setFontHeight(18);
	edit->setEditMultiLine(true);
	edit->setCaption("1");

}

void DemoKeeper::end()
{
}
