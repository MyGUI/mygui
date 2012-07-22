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

	class SeparatorListControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		SeparatorListControl();
		virtual ~SeparatorListControl();

		void setDataInfo(const std::string& _parentType, const std::string& _currentType, const std::string& _propertyName, const std::string& _propertyUnique);

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		ListBoxDataControl* mListBoxControl;
		std::string mParentType;
		std::string mPropertyForName;
		std::string mPropertyForUnique;
	};

}

#endif
