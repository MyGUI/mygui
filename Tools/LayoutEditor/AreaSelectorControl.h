/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _867025be_2d89_4729_af93_2b681ee958be_
#define _867025be_2d89_4729_af93_2b681ee958be_

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

}

#endif
