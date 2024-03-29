/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _3552b1d5_c4d6_4e3b_97b1_fbd07d4b4c38_
#define _3552b1d5_c4d6_4e3b_97b1_fbd07d4b4c38_

#include "PropertyControl.h"
#include "ColourPanel.h"

namespace tools
{

	class MYGUI_EXPORT_DLL PropertyColourControl : public PropertyControl
	{
	public:
		~PropertyColourControl() override;

	protected:
		void updateCaption() override;
		void updateProperty() override;
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void notifyEditTextChange(MyGUI::EditBox* _sender);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyPreviewColour(const MyGUI::Colour& _value);

		bool parseColour1(const std::string& _value, MyGUI::Colour& _resultValue);
		bool parseColour2(const std::string& _value, MyGUI::Colour& _resultValue);
		bool parseColour3(const std::string& _value, MyGUI::Colour& _resultValue);

		bool isValidate(MyGUI::Colour& _resultValue);
		MyGUI::UString getClearValue();
		void setColour(bool _validate);

		void updateSetProperty();

	private:
		MyGUI::TextBox* mName{nullptr};
		MyGUI::EditBox* mEdit{nullptr};
		MyGUI::Widget* mColour{nullptr};

		ColourPanel* mColourPanel{nullptr};

		MyGUI::Colour mCurrentColour;
		MyGUI::Colour mPreviewColour;
	};

}

#endif
