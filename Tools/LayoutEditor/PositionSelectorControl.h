/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _12226117_7e24_4498_9381_d659450157a7_
#define _12226117_7e24_4498_9381_d659450157a7_

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

}

#endif
