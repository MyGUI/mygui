/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _8b808213_c477_406c_ae46_1237d418557f_
#define _8b808213_c477_406c_ae46_1237d418557f_

#include "Dialog.h"
#include "Control.h"

namespace tools
{

	class MYGUI_EXPORT_DLL TextFieldControl : public Dialog, public Control
	{
	public:
		~TextFieldControl() override;

		void setCaption(const MyGUI::UString& _value);

		void setTextField(const MyGUI::UString& _value);
		MyGUI::UString getTextField();

		void setUserData(MyGUI::Any _data);

		void setCoord(const MyGUI::IntCoord& _value);

		template<typename ValueType>
		ValueType* getUserData(bool _throw = true)
		{
			return mMainWidget->getUserData<ValueType>(_throw);
		}

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

		void onDoModal() override;
		void onEndModal() override;

	private:
		void notifyOk(MyGUI::Widget* _sender);
		void notifyCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, std::string_view _buttonName);
		void notifyTextAccept(MyGUI::EditBox* _sender);
		void notifyRootKeyChangeFocus(MyGUI::Widget* _sender, bool _focus);

	private:
		MyGUI::EditBox* mText{nullptr};
		MyGUI::Button* mOk{nullptr};
		MyGUI::Button* mCancel{nullptr};
	};

}

#endif
