#ifndef __STRANGE_BUTTON_H__
#define __STRANGE_BUTTON_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	namespace factory { class StrangeButtonFactory; }

	class StrangeButton : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<StrangeButton>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		StrangeButton(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

	public:

		void setButtonPressed(bool _pressed)
		{
			if (mIsStatePressed == _pressed) return;
			mIsStatePressed = _pressed;
			updateButtonState();
		}

		bool getButtonPressed() { return mIsStatePressed; }

	protected:

		virtual void onMouseLostFocus(WidgetPtr _new);
		virtual void onMouseSetFocus(WidgetPtr _old);
		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);

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
		bool mIsPressed;
		// в фокусе ли кнопка
		bool mIsFocus;
		// статус кнопки нажата или нет
		bool mIsStatePressed;

	};

	typedef StrangeButton * StrangeButtonPtr;
}

#endif // __STRANGE_BUTTON_H__
