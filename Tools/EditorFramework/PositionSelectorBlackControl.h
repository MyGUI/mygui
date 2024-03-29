/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _1b53ad22_6e74_4d8a_aae1_17b0bd2a5d47_
#define _1b53ad22_6e74_4d8a_aae1_17b0bd2a5d47_

#include "SelectorControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL PositionSelectorBlackControl : public SelectorControl
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
	};

}

#endif
