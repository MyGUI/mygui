/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "OptionControl.h"

namespace tools
{

	OptionControl::OptionControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("OptionControl.layout", _parent),
		mOptionPathControl(nullptr)//,
		//mRegionPropertyControl(nullptr),
		//mRegionTextureControl(nullptr)
	{
		assignBase(mOptionPathControl, "OptionPathControl");
		//assignBase(mRegionPropertyControl, "RegionPropertyControl");
		//assignBase(mRegionTextureControl, "RegionTextureControl");
	}

	OptionControl::~OptionControl()
	{
	}

} // namespace tools
