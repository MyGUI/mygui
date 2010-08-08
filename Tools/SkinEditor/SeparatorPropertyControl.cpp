/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SeparatorPropertyControl.h"
#include "SkinManager.h"

namespace tools
{

	SeparatorPropertyControl::SeparatorPropertyControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SeparatorPropertyControl.layout", _parent)
	{
		assignBase(mPropertyVisibleControl, "PropertyVisible");
		assignBase(mPropertyPositionControl, "PropertyPosition");

		initialiseAdvisor();
	}

	SeparatorPropertyControl::~SeparatorPropertyControl()
	{
		shutdownAdvisor();
	}

	void SeparatorPropertyControl::updateSeparatorProperties()
	{
		SeparatorItem* separator = getCurrentSeparator();
		mPropertyVisibleControl->setProperty(separator != nullptr ? separator->getPropertySet()->getChild("Visible") : nullptr);
		mPropertyPositionControl->setProperty(separator != nullptr ? separator->getPropertySet()->getChild("Position") : nullptr);
	}

} // namespace tools
