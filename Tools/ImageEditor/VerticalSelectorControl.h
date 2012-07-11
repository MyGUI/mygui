/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __VERTICAL_SELECTOR_CONTROL_H__
#define __VERTICAL_SELECTOR_CONTROL_H__

#include "SelectorControl.h"

namespace tools
{

	class VerticalSelectorControl :
		public SelectorControl
	{
	public:
		VerticalSelectorControl(MyGUI::Widget* _parent);
		virtual ~VerticalSelectorControl();
	};

} // namespace tools

#endif // __VERTICAL_SELECTOR_CONTROL_H__
