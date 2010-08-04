/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinControl.h"

namespace tools
{

	SkinControl::SkinControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SkinControl.layout", _parent),
		mSkinListControl(nullptr),
		mSkinPropertyControl(nullptr),
		mSkinTextureControl(nullptr)
	{
		assignBase(mSkinListControl, "SkinListControl");
		assignBase(mSkinPropertyControl, "SkinPropertyControl");
		assignBase(mSkinTextureControl, "SkinTextureControl");
	}

	SkinControl::~SkinControl()
	{
	}

} // namespace tools
