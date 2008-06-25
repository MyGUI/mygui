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

	MyGUI::EditPtr edit = mGUI->createWidget<MyGUI::Edit>("EditStretch", MyGUI::IntCoord(100, 100, 340, 26), MyGUI::ALIGN_DEFAULT, "Main");
	edit->setTextAlign(MyGUI::ALIGN_HCENTER | MyGUI::ALIGN_VCENTER);
	edit->setCaption("ComplexProgress");//*/

	/*MyGUI::ButtonPtr button = mGUI->createWidget<MyGUI::Button>("ButtonSmall", MyGUI::IntCoord(100, 140, 100, 26), MyGUI::ALIGN_DEFAULT, "Main");
	button->setTextAlign(MyGUI::ALIGN_HCENTER | MyGUI::ALIGN_VCENTER);
	button->setCaption("ComplexProgress");//*/

}

void DemoKeeper::end()
{
}
