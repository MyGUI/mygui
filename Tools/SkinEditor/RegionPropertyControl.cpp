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
		RegionItem* separator = getCurrentRegion();
		mPropertyPositionControl->setProperty(separator != nullptr ? separator->getPropertySet()->getChild("Position") : nullptr);
		mPropertyVisibleControl->setProperty(separator != nullptr ? separator->getPropertySet()->getChild("Visible") : nullptr);
		mPropertyEnabledControl->setProperty(separator != nullptr ? separator->getPropertySet()->getChild("Enabled") : nullptr);
		mPropertyAlignControl->setProperty(separator != nullptr ? separator->getPropertySet()->getChild("Align") : nullptr);
		mPropertyRegionTypeControl->setProperty(separator != nullptr ? separator->getPropertySet()->getChild("RegionType") : nullptr);
	}

} // namespace tools
