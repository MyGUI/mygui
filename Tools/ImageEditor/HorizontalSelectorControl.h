/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __HORIZONTAL_SELECTOR_CONTROL_H__
#define __HORIZONTAL_SELECTOR_CONTROL_H__

#include "SelectorControl.h"

namespace tools
{

	class HorizontalSelectorControl :
		public SelectorControl
	{
	public:
		HorizontalSelectorControl(MyGUI::Widget* _parent);
		virtual ~HorizontalSelectorControl();
	};

} // namespace tools

#endif // __HORIZONTAL_SELECTOR_CONTROL_H__
