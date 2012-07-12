/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "MainPane.h"
#include "ControlUtility.h"

namespace tools
{

	MainPane::MainPane() :
		//wraps::BaseLayout("MainPane.layout"),
		//mSkinControl(nullptr),
		//mStatesControl(nullptr),
		//mSeparatorControl(nullptr),
		//mRegionControl(nullptr),
		mMainMenuControl(nullptr),
		mTab(nullptr)//,
		//mSkinListControl(nullptr)
	{
	}

	void MainPane::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::Initialise(_parent, _place, _layoutName);

		//assignBase(mSkinControl, "SkinControl");
		//assignBase(mStatesControl, "StateControl");
		//assignBase(mSeparatorControl, "SeparatorControl");
		//assignBase(mRegionControl, "RegionControl");
		assignBase(mMainMenuControl, "MainMenuControl");
		//assignBase(mSkinListControl, "SkinControl");

		assignWidget(mTab, "Tab");

		mTab->eventTabChangeSelect += MyGUI::newDelegate(this, &MainPane::notifyTabChangeSelect);

		notifyTabChangeSelect(mTab, 0);

		//components::ControlUtility::ControlInitialise(mMainWidget);
		//components::ControlUtility::ControlInitialise(this);
	}

	MainPane::~MainPane()
	{
		mTab->eventTabChangeSelect -= MyGUI::newDelegate(this, &MainPane::notifyTabChangeSelect);
	}

	void MainPane::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	void MainPane::notifyTabChangeSelect(MyGUI::TabControl* _sender, size_t _index)
	{
		//mSkinControl->setActivate(false);
		//mStatesControl->setActivate(false);
		//mSeparatorControl->setActivate(false);
		//mRegionControl->setActivate(false);

		//if (_index == 0)
			//mSkinControl->setActivate(true);
		//else if (_index == 1)
			//mStatesControl->setActivate(true);
		//else if (_index == 2)
			//mSeparatorControl->setActivate(true);
		//else if (_index == 3)
			//mRegionControl->setActivate(true);
	}

} // namespace tools
