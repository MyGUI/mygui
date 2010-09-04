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
		wraps::BaseLayout("SeparatorControl.layout", _parent),
		mSeparatorListControl(nullptr),
		mSeparatorPropertyControl(nullptr),
		mSeparatorTextureControl(nullptr)
	{
		assignBase(mSeparatorListControl, "SeparatorListControl");
		assignBase(mSeparatorPropertyControl, "SeparatorPropertyControl");
		assignBase(mSeparatorTextureControl, "SeparatorTextureControl");
	}

	SeparatorControl::~SeparatorControl()
	{
	}

} // namespace tools
