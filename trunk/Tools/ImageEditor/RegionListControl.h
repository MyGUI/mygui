/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _33e1ba1a_1c04_42c3_be29_00db6f6cb3a0_
#define _33e1ba1a_1c04_42c3_be29_00db6f6cb3a0_

#include "Control.h"
#include "Data.h"
#include "ListBoxDataControl.h"

namespace tools
{

	class RegionListControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		RegionListControl();
		virtual ~RegionListControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		ListBoxDataControl* mListBoxControl;
	};

}

#endif
