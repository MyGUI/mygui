/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _f1059563_afa2_4c08_ba11_3a60f1c91967_
#define _f1059563_afa2_4c08_ba11_3a60f1c91967_

#include "DataListBaseControl.h"

namespace tools
{

	class FontListControl : public DataListBaseControl
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
	};

}

#endif
