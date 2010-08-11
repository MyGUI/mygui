/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __AREA_SELECTOR_CONTROL_H__
#define __AREA_SELECTOR_CONTROL_H__

#include "SelectorControl.h"

namespace tools
{

	class AreaSelectorControl :
		public SelectorControl
	{
	public:
		AreaSelectorControl(MyGUI::Widget* _parent);
		virtual ~AreaSelectorControl();
	};

} // namespace tools

#endif // __AREA_SELECTOR_CONTROL_H__
