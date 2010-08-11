/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "RegionPropertyControl.h"
#include "SkinManager.h"

namespace tools
{

	RegionPropertyControl::RegionPropertyControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("RegionPropertyControl.layout", _parent)
	{
		assignBase(mPropertyVisibleControl, "PropertyVisible");
		assignBase(mPropertyPositionControl, "PropertyPosition");

		initialiseAdvisor();
	}

	RegionPropertyControl::~RegionPropertyControl()
	{
		shutdownAdvisor();
	}

	void RegionPropertyControl::updateRegionProperties()
	{
		RegionItem* separator = getCurrentRegion();
		mPropertyVisibleControl->setProperty(separator != nullptr ? separator->getPropertySet()->getChild("Visible") : nullptr);
		mPropertyPositionControl->setProperty(separator != nullptr ? separator->getPropertySet()->getChild("Position") : nullptr);
	}

} // namespace tools
