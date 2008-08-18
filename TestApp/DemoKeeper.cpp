/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "DemoKeeper.h"
#include "BasisManager.h"

void DemoKeeper::start()
{
	using namespace MyGUI;
	const IntSize & view = Gui::getInstance().getViewSize();
	const IntSize size(42, 75);

/*	MyGUI::SkinManager::getInstance().load("debug.skin");
#if MYGUI_DEBUG_MODE == 1
	MyGUI::InputManager::getInstance().setShowFocus(true);
#endif*/

}

void DemoKeeper::end()
{
}

