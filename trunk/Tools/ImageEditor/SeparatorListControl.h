/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _c102caee_550e_490f_ae66_4d1d56a92e6c_
#define _c102caee_550e_490f_ae66_4d1d56a92e6c_

#include "BaseLayout/BaseLayout.h"
#include "SkinItem.h"
#include "PropertyAdvisor.h"

/*namespace tools
{

	class SeparatorListControl :
		public wraps::BaseLayout,
		public PropertyAdvisor
	{
	public:
		SeparatorListControl(MyGUI::Widget* _parent);
		virtual ~SeparatorListControl();

	private:
		void notifyChangePosition(MyGUI::ListBox* _sender, size_t _index);
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		virtual void updateSeparatorProperties();
		virtual void updateSkinProperties();

		virtual void updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner);

		void updateList();

		void fillPresets();
		void updatePreset();

	private:
		MyGUI::UString mTypeName;

		MyGUI::ListBox* mList;
		MyGUI::ComboBox* mPresets;
	};

}*/

#endif
