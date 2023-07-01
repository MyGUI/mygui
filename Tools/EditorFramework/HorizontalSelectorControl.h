/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _fb36ada9_f808_498d_adbf_b028b77a119e_
#define _fb36ada9_f808_498d_adbf_b028b77a119e_

#include "SelectorControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL HorizontalSelectorControl : public SelectorControl
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
	};

}

#endif
