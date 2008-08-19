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
	const IntSize size(57, 53);

	StaticImagePtr image = Gui::getInstance().createWidget<StaticImage>("StaticImage", IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), ALIGN_DEFAULT, "Main");

	image->setImageTexture("skin_fl_AZ_new.png");
	image->addItemInfo("0.5 | 57 53 | 0 0");
	image->addItemInfo("0.5 | 57 53 | 0 53 | 0 106");
	image->addItemInfo("0.5 | 57 53 | 0 159 | 0 212");
	image->addItemNames("normal active select");
	image->setItemSelect("active");

/*	MyGUI::SkinManager::getInstance().load("debug.skin");
#if MYGUI_DEBUG_MODE == 1
	MyGUI::InputManager::getInstance().setShowFocus(true);
#endif*/

	/*"0.5 | 32 32 | 0 0 | 10 10"

		"0.5 | 32 32 | 0 0"
		"0.5 | 32 32 | 0 32 | 0 64"
		"0.5 | 32 32 | 0 69 | 0 128"*/

}

void DemoKeeper::end()
{
}

