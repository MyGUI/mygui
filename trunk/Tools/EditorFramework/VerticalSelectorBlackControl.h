/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _b22b8276_f81f_4b35_baca_fbda2e511467_
#define _b22b8276_f81f_4b35_baca_fbda2e511467_

#include "SelectorControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL VerticalSelectorBlackControl :
		public SelectorControl
	{
	public:
		VerticalSelectorBlackControl();
		virtual ~VerticalSelectorBlackControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);
	};

}

#endif
