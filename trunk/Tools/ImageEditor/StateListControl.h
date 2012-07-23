/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _f8ae0b3a_8708_4ee7_b996_3b3fcc61ad3f_
#define _f8ae0b3a_8708_4ee7_b996_3b3fcc61ad3f_

#include "Control.h"
#include "Data.h"
#include "ListBoxDataControl.h"

namespace tools
{

	class StateListControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		StateListControl();
		virtual ~StateListControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		ListBoxDataControl* mListBoxControl;
	};

}

#endif
