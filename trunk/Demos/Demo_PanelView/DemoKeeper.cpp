/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mView(nullptr),
		mPanelDirector(nullptr),
		mPanelDynamic(nullptr),
		mPanelStatic(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_PanelView");
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		getGUI()->load("Wallpaper0.layout");
		MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("BackHelp.layout");
		root.at(0)->findWidget("Text")->setCaption("Panel View control implementation.");

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
		mView = nullptr;
		delete mPanelDirector;
		mPanelDirector = nullptr;
		delete mPanelDynamic;
		mPanelDynamic = nullptr;
		delete mPanelStatic;
		mPanelStatic = nullptr;
	}

	void DemoKeeper::notifyChangePanels(int _key, size_t _value)
	{
		if (_key == EVENT_SHOW_STATIC)
		{
			mView->getPanelView()->setItemShow(mPanelStatic, _value != 0);
		}
		else if (_key == EVENT_SHOW_DYNAMIC)
		{
			mView->getPanelView()->setItemShow(mPanelDynamic, _value != 0);
		}
		else if (_key == EVENT_COUNT_DYNAMIC)
		{
			mPanelDynamic->setVisibleCount(_value);
		}
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
