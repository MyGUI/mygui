/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _463505b4_6ae5_4365_bf63_0f003a5dbde1_
#define _463505b4_6ae5_4365_bf63_0f003a5dbde1_

#include "SelectorControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL AreaSelectorControl : public SelectorControl
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
	};

}

#endif
