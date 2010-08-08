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
		mStatesControl(nullptr)
	{
		assignBase(mSkinControl, "SkinControl");
		assignBase(mStatesControl, "StatesControl");
		assignBase(mSeparatorControl, "SeparatorControl");
	}

	MainPane::~MainPane()
	{
	}

} // namespace tools
