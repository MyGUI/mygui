/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __REGION_SELECTOR_CONTROL_H__
#define __REGION_SELECTOR_CONTROL_H__

#include "SelectorControl.h"

namespace tools
{

	class RegionSelectorControl :
		public SelectorControl
	{
	public:
		RegionSelectorControl(MyGUI::Widget* _parent);
		virtual ~RegionSelectorControl();
	};

} // namespace tools

#endif // __REGION_SELECTOR_CONTROL_H__
