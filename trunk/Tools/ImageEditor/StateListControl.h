/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _8a4eac59_f042_4f5b_900d_d58201ea1791_
#define _8a4eac59_f042_4f5b_900d_d58201ea1791_

#include "DataListBaseControl.h"

namespace tools
{

	class StateListControl :
		public DataListBaseControl
	{
	public:
		StateListControl();
		virtual ~StateListControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);
	};

}

#endif
