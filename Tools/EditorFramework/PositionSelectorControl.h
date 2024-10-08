/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _2be1b24e_74ba_4e30_b328_cb77666a2677_
#define _2be1b24e_74ba_4e30_b328_cb77666a2677_

#include "SelectorControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL PositionSelectorControl : public SelectorControl
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
	};

}

#endif
