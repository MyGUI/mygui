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
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../Media/Demos/Demo_PanelView", "FileSystem", "General");

	using namespace MyGUI;
	const IntSize & view = Gui::getInstance().getViewSize();
	const IntSize size(600, 300);

	mView.initialise();

	mPanelDirector.eventChangePanels = MyGUI::newDelegate(this, &DemoKeeper::notifyChangePanels);
	mView.addItem(&mPanelDirector);
	mView.addItem(&mPanelDynamic);
	mView.addItem(&mPanelStatic);

}

void DemoKeeper::end()
{
	mView.shutdown();
}

void DemoKeeper::notifyChangePanels(int _key, size_t _value)
{
	if (_key == EVENT_SHOW_STATIC) {
		mView.setItemShow(&mPanelStatic, (bool)_value);
	}
	else if (_key == EVENT_SHOW_DYNAMIC) {
		mView.setItemShow(&mPanelDynamic, (bool)_value);
	}
	else if (_key == EVENT_COUNT_DYNAMIC) {
		mPanelDynamic.setVisibleCount(_value);
	}
}
