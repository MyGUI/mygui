/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "DemoKeeper.h"
#include <fstream>

DemoKeeper::DemoKeeper() :
	mGUI(0),
	mWidth(1), mHeight(1)
{
}

MyGUI::WidgetPtr but = 0;

void DemoKeeper::move(int _left, int _top)
{
	if (but != 0) but->setPosition(_left - 100, _top - 100);
}

void DemoKeeper::start(MyGUI::Gui * _gui, size_t _width, size_t _height)
{
    mGUI = _gui;
    mWidth = _width;
    mHeight = _height;

	/*for (size_t pos=0; pos<1; pos++)*/
	MyGUI::WidgetPtr wid = mGUI->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(300, 100, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");//*/

	wid = wid->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(-10, 10, 300, 280), MyGUI::ALIGN_DEFAULT);//*/

	// это скин с одним сабскином
	but = wid->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(10, 10, 60, 26), MyGUI::ALIGN_LEFT | MyGUI::ALIGN_BOTTOM );

	// это скин с 3 сабскинами
	//but = wid->createWidget<MyGUI::Widget>("Button", MyGUI::IntCoord(10, 10, 60, 26), MyGUI::ALIGN_LEFT | MyGUI::ALIGN_BOTTOM );

	//wid->setPosition(100, 100, 300, 300);
	//MyGUI::MYGUI_OUT("void Defyhs fgmoKeeper::start(MyGUIwer::Gui * _gui, size_t _width, size_t _height)");

}

void DemoKeeper::end()
{
}
