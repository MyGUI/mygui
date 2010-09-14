/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
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
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper0.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("BackHelp.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::StaticText>()->setCaption("Panel View control implementation.");

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
			mPanelStatic->setVisible(_value != 0);
		}
		else if (_key == EVENT_SHOW_DYNAMIC)
		{
			mPanelDynamic->setVisible(_value != 0);
		}
		else if (_key == EVENT_COUNT_DYNAMIC)
		{
			mPanelDynamic->setVisibleCount(_value);
		}
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
