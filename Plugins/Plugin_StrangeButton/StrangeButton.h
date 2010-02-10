/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/
#ifndef __STRANGE_BUTTON_H__
#define __STRANGE_BUTTON_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_StaticText.h"

namespace plugin
{

	class StrangeButton :
		public MyGUI::StaticText
	{
		MYGUI_RTTI_DERIVED( StrangeButton )

	public:
		StrangeButton();

		//! OLD Set button check state
		void setButtonPressed(bool _value) { setStateCheck(_value); }
		//! OLD Get buton check
		bool getButtonPressed() { return getStateCheck(); }

		//! Set button check state
		void setStateCheck(bool _value);

		//! Get buton check
		bool getStateCheck() { return mIsStateCheck; }

	/*internal:*/
		virtual void _initialise(MyGUI::WidgetStyle _style, const MyGUI::IntCoord& _coord, MyGUI::Align _align, MyGUI::ResourceSkin* _info, MyGUI::Widget* _parent, MyGUI::ICroppedRectangle * _croppedParent, MyGUI::IWidgetCreator * _creator, const std::string& _name);

		void _setMouseFocus(bool _focus);

	protected:
		virtual ~StrangeButton();

		virtual void baseChangeWidgetSkin(MyGUI::ResourceSkin* _info);

		virtual void onMouseLostFocus(MyGUI::Widget* _new);
		virtual void onMouseSetFocus(MyGUI::Widget* _old);
		virtual void onMouseButtonPressed(int _left, int _top, MyGUI::MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MyGUI::MouseButton _id);

		virtual void baseUpdateEnable();


	private:
		void updateButtonState();

		void shutdownWidgetSkin();
		void initialiseWidgetSkin(MyGUI::ResourceSkin* _info);

	private:
		// нажата ли кнопка
		bool mIsMousePressed;
		// в фокусе ли кнопка
		bool mIsMouseFocus;
		// статус кнопки нажата или нет
		bool mIsStateCheck;

	};

} // namespace plugin

#endif // __STRANGE_BUTTON_H__
