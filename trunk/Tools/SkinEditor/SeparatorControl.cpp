/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SeparatorControl.h"

namespace tools
{

	SeparatorControl::SeparatorControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SeparatorsControl.layout", _parent),
		mSeparatorListControl(nullptr)//,
		//mStatePropertyControl(nullptr),
		//mStateTextureControl(nullptr)
	{
		assignBase(mSeparatorListControl, "SeparatorListControl");
		//assignBase(mStatePropertyControl, "StatePropertyControl");
		//assignBase(mStateTextureControl, "StateTextureControl");
	}

	SeparatorControl::~SeparatorControl()
	{
	}

} // namespace tools
