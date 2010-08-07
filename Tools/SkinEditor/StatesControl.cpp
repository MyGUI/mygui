/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "StatesControl.h"

namespace tools
{

	StatesControl::StatesControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("StatesControl.layout", _parent),
		mStatesListControl(nullptr)//,
		//mSkinPropertyControl(nullptr),
		//mSkinTextureControl(nullptr)
	{
		assignBase(mStatesListControl, "StatesListControl");
		//assignBase(mSkinPropertyControl, "SkinPropertyControl");
		//assignBase(mSkinTextureControl, "SkinTextureControl");
	}

	StatesControl::~StatesControl()
	{
	}

} // namespace tools
