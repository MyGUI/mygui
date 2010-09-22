/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "MainPane.h"

namespace tools
{

	MainPane::MainPane() :
		wraps::BaseLayout("MainPane.layout"),
		mSkinControl(nullptr),
		mStatesControl(nullptr),
		mSeparatorControl(nullptr),
		mRegionControl(nullptr),
		mMainMenuControl(nullptr)
	{
		assignBase(mSkinControl, "SkinControl");
		assignBase(mStatesControl, "StateControl");
		assignBase(mSeparatorControl, "SeparatorControl");
		assignBase(mRegionControl, "RegionControl");
		assignBase(mMainMenuControl, "MainMenuControl");
	}

	MainPane::~MainPane()
	{
	}

	void MainPane::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

} // namespace tools
