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

	//MyGUI::ButtonPtr button = mGUI->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(100, 100, 340, 26), MyGUI::ALIGN_DEFAULT, "Main");

	/*MyGUI::ButtonPtr button = mGUI->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(100, 100, 340, 26), MyGUI::ALIGN_DEFAULT, "Main");
	button->setCaption("Button");//*/

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

}

void DemoKeeper::end()
{
}
