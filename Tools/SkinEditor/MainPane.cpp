/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "MainPane.h"

namespace tools
{

	MainPane::MainPane() :
		wraps::BaseLayout("MainPane.layout"),
		mSkinControl(nullptr),
		mStatesControl(nullptr),
		mSeparatorControl(nullptr),
		mRegionControl(nullptr),
		mMainMenuControl(nullptr),
		mOptionControl(nullptr)
	{
		assignBase(mSkinControl, "SkinControl");
		assignBase(mStatesControl, "StateControl");
		assignBase(mSeparatorControl, "SeparatorControl");
		assignBase(mRegionControl, "RegionControl");
		assignBase(mMainMenuControl, "MainMenuControl");
		assignBase(mOptionControl, "OptionControl");
	}

	MainPane::~MainPane()
	{
	}

} // namespace tools
