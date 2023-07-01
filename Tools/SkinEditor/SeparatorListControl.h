/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _655d5259_42ad_447e_a3bc_17890df040c2_
#define _655d5259_42ad_447e_a3bc_17890df040c2_

#include "Control.h"
#include "Data.h"
#include "ListBoxDataControl.h"

namespace tools
{

	class SeparatorListControl : public Control, public sigslot::has_slots<>
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		ListBoxDataControl* mListBoxControl{nullptr};
	};

}

#endif
