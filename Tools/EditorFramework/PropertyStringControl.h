/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _ac3769fd_b132_461a_b607_737e76f02a6e_
#define _ac3769fd_b132_461a_b607_737e76f02a6e_

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL PropertyStringControl : public PropertyControl
	{
	public:
		~PropertyStringControl() override;

	protected:
		void updateCaption() override;
		void updateProperty() override;
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void notifyEditTextChange(MyGUI::EditBox* _sender);

		MyGUI::UString getClearValue();

	private:
		MyGUI::TextBox* mName{nullptr};
		MyGUI::EditBox* mEdit{nullptr};
	};

}

#endif
