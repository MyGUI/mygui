#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Prerequest.h"
#include "ButtonFactory.h"
#include "Widget.h"

namespace MyGUI
{

	class Button;
	typedef Button* ButtonPtr;

	class _MyGUIExport Button : public Widget
	{
		// для вызова закрытого конструктора
		friend factory::ButtonFactory;

	protected:
		Button(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:

		inline void setButtonPressed(bool _pressed)
		{
			if (mIsStatePressed == _pressed) return;
			mIsStatePressed = _pressed;
			updateButtonState();
		}

		inline bool getButtonPressed() {return mIsStatePressed;}

	protected:

		virtual void _onMouseLostFocus(WidgetPtr _new);
		virtual void _onMouseSetFocus(WidgetPtr _old);
		virtual void _onMouseButtonPressed(bool _left);
		virtual void _onMouseButtonReleased(bool _left);

		inline void updateButtonState()
		{
			if (mIsFocus) {
				if (mIsPressed || mIsStatePressed) setState("select");
				else setState("active");
			} else {
				if (mIsPressed || mIsStatePressed) setState("pressed");
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

	}; // class Button : public Widget

} // namespace MyGUI

#endif // __BUTTON_H__