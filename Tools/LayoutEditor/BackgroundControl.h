/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/

#ifndef _dec67650_6f1e_40e7_95a3_8311e4727290_
#define _dec67650_6f1e_40e7_95a3_8311e4727290_

#include <MyGUI.h>
#include "Control.h"

namespace tools
{

	class BackgroundControlLE : public Control
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
	};

}

#endif
