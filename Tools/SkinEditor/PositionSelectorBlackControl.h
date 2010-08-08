/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __POSITION_SELECTOR_BLACK_CONTROL_H__
#define __POSITION_SELECTOR_BLACK_CONTROL_H__

#include "SelectorControl.h"

namespace tools
{

	class PositionSelectorBlackControl :
		public SelectorControl
	{
	public:
		PositionSelectorBlackControl(MyGUI::Widget* _parent);
		virtual ~PositionSelectorBlackControl();
	};

} // namespace tools

#endif // __POSITION_SELECTOR_BLACK_CONTROL_H__
