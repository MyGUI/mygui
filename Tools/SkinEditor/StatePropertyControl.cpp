/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "StatePropertyControl.h"
#include "SkinManager.h"

namespace tools
{

	StatePropertyControl::StatePropertyControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("StatePropertyControl.layout", _parent)
	{
		assignBase(mPropertyVisibleControl, "PropertyVisible");
		assignBase(mPropertyPositionControl, "PropertyPosition");

		initialiseAdvisor();
	}

	StatePropertyControl::~StatePropertyControl()
	{
		shutdownAdvisor();
	}

	void StatePropertyControl::updateStateProperties()
	{
		StateItem* state = getCurrentState();
		mPropertyVisibleControl->setProperty(state != nullptr ? state->getPropertySet()->getChild("Visible") : nullptr);
		mPropertyPositionControl->setProperty(state != nullptr ? state->getPropertySet()->getChild("Position") : nullptr);
	}

} // namespace tools
