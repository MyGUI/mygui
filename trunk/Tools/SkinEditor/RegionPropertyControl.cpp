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
		wraps::BaseLayout("RegionPropertyControl.layout", _parent),
		mPropertyPositionControl(nullptr),
		mPropertyVisibleControl(nullptr),
		mPropertyEnabledControl(nullptr),
		mPropertyAlignControl(nullptr),
		mPropertyRegionTypeControl(nullptr)
	{
		assignBase(mPropertyPositionControl, "PropertyPosition");
		assignBase(mPropertyVisibleControl, "PropertyVisible");
		assignBase(mPropertyEnabledControl, "PropertyEnabled");
		assignBase(mPropertyAlignControl, "PropertyAlign");
		assignBase(mPropertyRegionTypeControl, "PropertyRegionType");

		initialiseAdvisor();
	}

	RegionPropertyControl::~RegionPropertyControl()
	{
		shutdownAdvisor();
	}

	void RegionPropertyControl::updateRegionProperties()
	{
		RegionItem* region = getCurrentRegion();
		mPropertyPositionControl->setProperty(region != nullptr ? region->getPropertySet()->getChild("Position") : nullptr);
		mPropertyVisibleControl->setProperty(region != nullptr ? region->getPropertySet()->getChild("Visible") : nullptr);
		mPropertyEnabledControl->setProperty(region != nullptr ? region->getPropertySet()->getChild("Enabled") : nullptr);
		mPropertyAlignControl->setProperty(region != nullptr ? region->getPropertySet()->getChild("Align") : nullptr);
		mPropertyRegionTypeControl->setProperty(region != nullptr ? region->getPropertySet()->getChild("RegionType") : nullptr);
	}

} // namespace tools
