/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _b0713d87_24ee_44cf_bee9_c96494be178e_
#define _b0713d87_24ee_44cf_bee9_c96494be178e_

#include "Control.h"
#include "sigslot.h"
#include "Data.h"

namespace tools
{
	class ListBoxDataControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		ListBoxDataControl();
		virtual ~ListBoxDataControl();

		virtual void Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index);
		void notifyChangeDataSelector(Data* _data, bool _changeOnlySelection);

		void invalidateList();
		void invalidateSelection();

	private:
		MyGUI::ListBox* mListBox;
		Data* mParentData;
		size_t mLastIndex;
	};
}

#endif
