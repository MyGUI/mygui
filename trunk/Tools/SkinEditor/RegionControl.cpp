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
		mRegionListControl(nullptr)//,
		//mSkinPropertyControl(nullptr),
		//mSkinTextureControl(nullptr)
	{
		assignBase(mRegionListControl, "RegionListControl");
		//assignBase(mSkinPropertyControl, "SkinPropertyControl");
		//assignBase(mSkinTextureControl, "SkinTextureControl");
	}

	RegionControl::~RegionControl()
	{
	}

} // namespace tools
