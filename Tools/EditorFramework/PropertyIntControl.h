/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _5cec3584_8871_43a7_a9da_ae1fae723f32_
#define _5cec3584_8871_43a7_a9da_ae1fae723f32_

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL PropertyIntControl : public PropertyControl
	{
	public:
		~PropertyIntControl() override;

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
