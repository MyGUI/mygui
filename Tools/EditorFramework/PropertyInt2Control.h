/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _b3697c6a_b56b_4edd_b9e0_1f2e871d0da1_
#define _b3697c6a_b56b_4edd_b9e0_1f2e871d0da1_

#include "PropertyControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL PropertyInt2Control : public PropertyControl
	{
	public:
		~PropertyInt2Control() override;

	protected:
		void updateCaption() override;
		void updateProperty() override;
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void notifyEditTextChange(MyGUI::EditBox* _sender);

		bool isValidate();
		MyGUI::UString getClearValue();
		void setColour(bool _validate);

	private:
		MyGUI::TextBox* mName{nullptr};
		MyGUI::EditBox* mEdit{nullptr};
	};

}

#endif
