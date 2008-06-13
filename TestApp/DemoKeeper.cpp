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

	MyGUI::FontManager::getInstance().saveFontTexture("MyGUI_CoreFont.18", "MyGUI_CoreFont.18.png");

}

void DemoKeeper::end()
{
}
