/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _cfb9d18e_b6dc_4543_827d_ee5f6fa93fdd_
#define _cfb9d18e_b6dc_4543_827d_ee5f6fa93fdd_

#include "SelectorControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL HorizontalSelectorBlackControl : public SelectorControl
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
	};

}

#endif
