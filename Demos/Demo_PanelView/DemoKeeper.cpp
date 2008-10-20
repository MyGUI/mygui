/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{
        //base::BaseManager::getInstance().addResourceLocation("../../Media/LayoutEditor/Panels");
        base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_PanelView");
        base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");

		mView.initialise();

		mPanelDirector.eventChangePanels = MyGUI::newDelegate(this, &DemoKeeper::notifyChangePanels);
		mView.addItem(&mPanelDirector);
		mView.addItem(&mPanelDynamic);
		mView.addItem(&mPanelStatic);

	}

	void DemoKeeper::destroyScene()
	{
		mView.shutdown();
	}

	void DemoKeeper::notifyChangePanels(int _key, size_t _value)
	{
		if (_key == EVENT_SHOW_STATIC) {
			mView.setItemShow(&mPanelStatic, _value != 0);
		}
		else if (_key == EVENT_SHOW_DYNAMIC) {
			mView.setItemShow(&mPanelDynamic, _value != 0);
		}
		else if (_key == EVENT_COUNT_DYNAMIC) {
			mPanelDynamic.setVisibleCount(_value);
		}
	}

} // namespace demo
