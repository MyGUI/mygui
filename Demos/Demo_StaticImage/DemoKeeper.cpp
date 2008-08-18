/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"
#include "BasisManager.h"

void DemoKeeper::start()
{
	// загружаем ресурсы для демо
	// потом сделать и для мака
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../Media/Demos/Demo_StaticImage", "FileSystem", "General");

	using namespace MyGUI;
	const IntSize & view = Gui::getInstance().getViewSize();
	const IntSize size(42, 75);

	StaticImagePtr image = Gui::getInstance().createWidget<StaticImage>("StaticImage", IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), ALIGN_DEFAULT, "Main");
	image->setImageTexture("child.png");
	image->addItem(MyGUI::IntCoord(0, 0, 42, 75));
	image->addItemFrame(0, MyGUI::IntCoord(42, 0, 42, 75));
	image->addItemFrame(0, MyGUI::IntCoord(42*2, 0, 42, 75));
	image->addItemFrame(0, MyGUI::IntCoord(42*3, 0, 42, 75));
	image->addItemFrameDublicate(0, 0);
	image->addItemFrameDublicate(0, 3);
	image->setItemFrameRate(0, 0.15);
	image->setItemSelect(0);

}

void DemoKeeper::end()
{
}
