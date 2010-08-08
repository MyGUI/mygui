/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __POSITION_SELECTOR_CONTROL_H__
#define __POSITION_SELECTOR_CONTROL_H__

#include "SelectorControl.h"

namespace tools
{

	class PositionSelectorControl :
		public SelectorControl
	{
	public:
		PositionSelectorControl(MyGUI::Widget* _parent);
		virtual ~PositionSelectorControl();
	};

} // namespace tools

#endif // __POSITION_SELECTOR_CONTROL_H__
