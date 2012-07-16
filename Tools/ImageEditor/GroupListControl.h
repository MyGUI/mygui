/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _052ce52a_e3c6_40a8_ba07_acf0ead1d367_
#define _052ce52a_e3c6_40a8_ba07_acf0ead1d367_

#include "DataListBaseControl.h"

namespace tools
{

	class GroupListControl :
		public DataListBaseControl
	{
	public:
		GroupListControl();
		virtual ~GroupListControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);
	};

}

#endif
