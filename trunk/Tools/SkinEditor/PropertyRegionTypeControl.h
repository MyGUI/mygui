/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _cd340497_c8dc_4b2e_bfb8_49ae5952eddf_
#define _cd340497_c8dc_4b2e_bfb8_49ae5952eddf_

#include "PropertyControl.h"

namespace tools
{

	class PropertyRegionTypeControl :
		public PropertyControl
	{
	public:
		PropertyRegionTypeControl();
		virtual ~PropertyRegionTypeControl();

	protected:
		virtual void updateCaption();
		virtual void updateProperty();
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		size_t getComboIndex(const MyGUI::UString& _name);

	private:
		MyGUI::TextBox* mName;
		MyGUI::ComboBox* mComboBox;
	};

}

#endif
