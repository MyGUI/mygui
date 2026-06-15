/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _bf2aa111_a57e_464d_9f75_f102b5cac64f_
#define _bf2aa111_a57e_464d_9f75_f102b5cac64f_

#include "DataListBaseControl.h"

namespace tools
{

	class FrameListControl : public DataListBaseControl
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
	};

}

#endif
