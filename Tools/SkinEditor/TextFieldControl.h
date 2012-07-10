/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __TEXT_FIELD_CONTROL_H__
#define __TEXT_FIELD_CONTROL_H__

#include "Tools/Dialog.h"

namespace tools
{
	class TextFieldControl :
		public Dialog
	{
	public:
		TextFieldControl();
		virtual ~TextFieldControl();

		void setCaption(const MyGUI::UString& _value);

		void setTextField(const MyGUI::UString& _value);
		MyGUI::UString getTextField();

		void setUserData(MyGUI::Any _data);

		void setCoord(const MyGUI::IntCoord& _value);

		template <typename ValueType>
		ValueType* getUserData(bool _throw = true)
		{
			return mMainWidget->getUserData<ValueType>(_throw);
		}

	protected:
		virtual void onDoModal();
		virtual void onEndModal();

	private:
		void notifyOk(MyGUI::Widget* _sender);
		void notifyCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _buttonName);
		void notifyTextAccept(MyGUI::EditBox* _sender);
		void notifyRootKeyChangeFocus(MyGUI::Widget* _sender, bool _focus);

	private:
		MyGUI::EditBox* mText;
		MyGUI::Button* mOk;
		MyGUI::Button* mCancel;
	};

} // namespace tools

#endif // __TEXT_FIELD_CONTROL_H__
