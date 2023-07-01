/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _bee00f7e_81b1_4fb4_9da0_7e52abb5d087_
#define _bee00f7e_81b1_4fb4_9da0_7e52abb5d087_

#include "PropertyControl.h"

namespace tools
{

	class PropertyRegionTextTypeControl : public PropertyControl
	{
	public:
		~PropertyRegionTextTypeControl() override;

	protected:
		void updateCaption() override;
		void updateProperty() override;
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		size_t getComboIndex(const MyGUI::UString& _name);

	private:
		MyGUI::TextBox* mName{nullptr};
		MyGUI::ComboBox* mComboBox{nullptr};
	};

}

#endif
