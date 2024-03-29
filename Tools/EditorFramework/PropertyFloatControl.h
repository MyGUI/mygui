/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _ecbac729_1c23_443a_879c_3fb6ec33eabb_
#define _ecbac729_1c23_443a_879c_3fb6ec33eabb_

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL PropertyFloatControl : public PropertyControl
	{
	public:
		~PropertyFloatControl() override;

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
