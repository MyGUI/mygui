/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "RegionControl.h"

namespace tools
{

	RegionControl::RegionControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("RegionControl.layout", _parent),
		mRegionListControl(nullptr),
		mRegionPropertyControl(nullptr),
		mRegionTextureControl(nullptr)
	{
		assignBase(mRegionListControl, "RegionListControl");
		assignBase(mRegionPropertyControl, "RegionPropertyControl");
		//assignBase(mRegionTextureControl, "RegionTextureControl");
	}

	RegionControl::~RegionControl()
	{
	}

} // namespace tools
