/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinPropertyControl.h"
#include "SkinManager.h"

namespace tools
{

	SkinPropertyControl::SkinPropertyControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SkinPropertyControl.layout", _parent),
		mPropertyCoordControl(nullptr),
		mPropertyTexturesControl(nullptr)
	{
		assignBase(mPropertyCoordControl, "PropertyCoord");
		assignBase(mPropertyTexturesControl, "PropertyTextures");

		initialiseAdvisor();
	}

	SkinPropertyControl::~SkinPropertyControl()
	{
		shutdownAdvisor();
	}

	void SkinPropertyControl::updateSkinProperties()
	{
		SkinItem* skin = getCurrentSkin();
		mPropertyCoordControl->setProperty(skin != nullptr ? skin->getPropertySet()->getChild("Coord") : nullptr);
		mPropertyTexturesControl->setProperty(skin != nullptr ? skin->getPropertySet()->getChild("Texture") : nullptr);
	}

} // namespace tools
