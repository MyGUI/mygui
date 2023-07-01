/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _af1ab5ed_a8c4_4aac_a94f_199b6879b33e_
#define _af1ab5ed_a8c4_4aac_a94f_199b6879b33e_

#include "PropertyControl.h"

namespace tools
{

	class PropertyFontHintingControl : public PropertyControl
	{
	public:
		~PropertyFontHintingControl() override;

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
