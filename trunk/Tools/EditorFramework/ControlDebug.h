/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef _09fd0384_b601_4697_9fd3_c105b4fe2058_
#define _09fd0384_b601_4697_9fd3_c105b4fe2058_

#include "Control.h"

namespace tools
{
	class MYGUI_EXPORT_DLL ControlDebug :
		public Control
	{
	public:
		ControlDebug();
		virtual ~ControlDebug();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);
	};

}

#endif
