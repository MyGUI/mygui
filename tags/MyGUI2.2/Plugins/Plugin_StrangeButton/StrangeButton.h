/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/
#ifndef __STRANGE_BUTTON_H__
#define __STRANGE_BUTTON_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace plugin
{

	namespace factory { class StrangeButtonFactory; }

	class StrangeButton : public MyGUI::Widget
	{
		// для вызова закрытого конструктора
		friend class MyGUI::factory::BaseWidgetFactory<StrangeButton>;

		MYGUI_RTTI_CHILD_HEADER( StrangeButton, MyGUI::Widget );

	protected:
		StrangeButton(MyGUI::WidgetStyle _style, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const MyGUI::WidgetSkinInfoPtr _info, MyGUI::WidgetPtr _parent, MyGUI::ICroppedRectangle * _croppedParent, MyGUI::IWidgetCreator * _creator, const std::string & _name);

	public:

		void setButtonPressed(bool _pressed)
		{
			if (mIsStateCheck == _pressed) return;
			mIsStateCheck = _pressed;
			updateButtonState();
		}

		bool getButtonPressed() { return mIsStateCheck; }

	protected:

		virtual void onMouseLostFocus(MyGUI::WidgetPtr _new);
		virtual void onMouseSetFocus(MyGUI::WidgetPtr _old);
		virtual void onMouseButtonPressed(int _left, int _top, MyGUI::MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MyGUI::MouseButton _id);

		void updateButtonState()
		{
			if (mIsStateCheck) {
				if (!mEnabled) { if (!setState("disabled_checked")) setState("disabled"); }
				else if (mIsMousePressed) { if (!setState("pushed_checked")) setState("pushed"); }
				else if (mIsMouseFocus) { if (!setState("highlighted_checked")) setState("pushed"); }
				else setState("normal_checked");
			}
			else {
				if (!mEnabled) setState("disabled");
				else if (mIsMousePressed) setState("pushed");
				else if (mIsMouseFocus) setState("highlighted");
				else setState("normal");
			}
		}


	private:
		// нажата ли кнопка
		bool mIsMousePressed;
		// в фокусе ли кнопка
		bool mIsMouseFocus;
		// статус кнопки нажата или нет
		bool mIsStateCheck;

	};

	typedef StrangeButton * StrangeButtonPtr;

} // namespace plugin

#endif // __STRANGE_BUTTON_H__
