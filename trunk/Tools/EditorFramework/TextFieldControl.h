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

	class MYGUI_EXPORT_DLL TextFieldControl :
		public Dialog,
		public Control
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
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

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

}

#endif
