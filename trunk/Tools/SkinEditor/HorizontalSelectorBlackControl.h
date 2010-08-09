/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __HORIZONTAL_SELECTOR_BLACK_CONTROL_H__
#define __HORIZONTAL_SELECTOR_BLACK_CONTROL_H__

#include "SelectorControl.h"

namespace tools
{

	class HorizontalSelectorBlackControl :
		public SelectorControl
	{
	public:
		HorizontalSelectorBlackControl(MyGUI::Widget* _parent);
		virtual ~HorizontalSelectorBlackControl();
	};

} // namespace tools

#endif // __HORIZONTAL_SELECTOR_BLACK_CONTROL_H__
