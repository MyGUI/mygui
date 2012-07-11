/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __VERTICAL_SELECTOR_BLACK_CONTROL_H__
#define __VERTICAL_SELECTOR_BLACK_CONTROL_H__

#include "SelectorControl.h"

namespace tools
{

	class VerticalSelectorBlackControl :
		public SelectorControl
	{
	public:
		VerticalSelectorBlackControl(MyGUI::Widget* _parent);
		virtual ~VerticalSelectorBlackControl();
	};

} // namespace tools

#endif // __VERTICAL_SELECTOR_BLACK_CONTROL_H__
