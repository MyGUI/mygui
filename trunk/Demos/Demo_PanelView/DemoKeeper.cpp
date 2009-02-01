/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{
		//base::BaseManager::getInstance().addResourceLocation("../../Media/LayoutEditor/Panels");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_PanelView");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		base::BaseManager::getInstance().setDescriptionText("Panel View control implementation.");

		mView = new PanelViewWindow();
		mPanelDirector = new PanelDirector();
		mPanelDynamic = new PanelDynamic();
		mPanelStatic = new PanelStatic();

		mPanelDirector->eventChangePanels = MyGUI::newDelegate(this, &DemoKeeper::notifyChangePanels);
		mView->getPanelView()->addItem(mPanelDirector);
		mView->getPanelView()->addItem(mPanelDynamic);
		mView->getPanelView()->addItem(mPanelStatic);
	}

	void DemoKeeper::destroyScene()
	{
		mView->getPanelView()->removeAllItems();
		delete mView;

		delete mPanelDirector;
		delete mPanelDynamic;
		delete mPanelStatic;
	}

	void DemoKeeper::notifyChangePanels(int _key, size_t _value)
	{
		if (_key == EVENT_SHOW_STATIC) {
			mView->getPanelView()->setItemShow(mPanelStatic, _value != 0);
		}
		else if (_key == EVENT_SHOW_DYNAMIC) {
			mView->getPanelView()->setItemShow(mPanelDynamic, _value != 0);
		}
		else if (_key == EVENT_COUNT_DYNAMIC) {
			mPanelDynamic->setVisibleCount(_value);
		}
	}

} // namespace demo
