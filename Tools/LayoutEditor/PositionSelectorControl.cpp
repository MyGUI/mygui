/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "PositionSelectorControl.h"

namespace tools
{

	PositionSelectorControl::PositionSelectorControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("PositionSelectorControl.layout", _parent)
	{
	}

	PositionSelectorControl::~PositionSelectorControl()
	{
	}

	void PositionSelectorControl::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	void PositionSelectorControl::setCoord(const MyGUI::IntCoord& _value)
	{
		mMainWidget->setCoord(_value);
	}

} // namespace tools
