/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _87448eb4_88e7_4617_9ee2_7dbce526cbcf_
#define _87448eb4_88e7_4617_9ee2_7dbce526cbcf_

#include "DataListBaseControl.h"

namespace tools
{

	class IndexListControl : public DataListBaseControl
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
	};

}

#endif
