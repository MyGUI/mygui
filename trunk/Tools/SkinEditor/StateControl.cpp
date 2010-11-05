/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "StateControl.h"

namespace tools
{

	StatesControl::StatesControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("StateControl.layout", _parent),
		mStatesListControl(nullptr),
		mStatePropertyControl(nullptr),
		mStateTextureControl(nullptr)
	{
		assignBase(mStatesListControl, "StateListControl");
		assignBase(mStatePropertyControl, "StatePropertyControl");
		assignBase(mStateTextureControl, "StateTextureControl");
	}

	StatesControl::~StatesControl()
	{
	}

	void StatesControl::setActivate(bool _value)
	{
		mStateTextureControl->setActivate(_value);
	}

} // namespace tools
