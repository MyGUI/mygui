/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef __LIST_BOX_CONTROL_H__
#define __LIST_BOX_CONTROL_H__

#include <MyGUI.h>
#include "ItemBox/BaseItemBox.h"
#include "ListBoxItemControl.h"

namespace tools
{
	class ListBoxControl :
		public wraps::BaseItemBox<ListBoxItemControl>
	{
	public:
		ListBoxControl(MyGUI::Widget* _parent);
		virtual ~ListBoxControl();
	};
}

#endif
