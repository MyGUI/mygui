/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _48318828_b4a4_495c_a50f_73a810b59e74_
#define _48318828_b4a4_495c_a50f_73a810b59e74_

#include "SelectorControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL VerticalSelectorControl : public SelectorControl
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
	};

}

#endif
