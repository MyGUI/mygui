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
		wraps::BaseLayout("StatePropertyControl.layout", _parent),
		mPropertyVisibleControl(nullptr),
		mPropertyPositionControl(nullptr),
		mPropertyColourControl(nullptr),
		mPropertyOffsetControl(nullptr)
	{
		assignBase(mPropertyVisibleControl, "PropertyVisible");
		assignBase(mPropertyPositionControl, "PropertyPosition");
		assignBase(mPropertyColourControl, "PropertyTextColour");
		assignBase(mPropertyOffsetControl, "PropertyTextOffset");

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
		mPropertyColourControl->setProperty(state != nullptr ? state->getPropertySet()->getChild("TextColour") : nullptr);
		mPropertyOffsetControl->setProperty(state != nullptr ? state->getPropertySet()->getChild("TextShift") : nullptr);
	}

} // namespace tools
