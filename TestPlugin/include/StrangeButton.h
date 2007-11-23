#ifndef __STRANGE_BUTTON_H__
#define __STRANGE_BUTTON_H__

#include "StrangeButtonFactory.h"
#include "Widget.h"

namespace MyGUI
{

	class StrangeButton : public Widget
	{
		// для вызова закрытого конструктора
		friend factory::StrangeButtonFactory;

	protected:
		StrangeButton(int _x, int _y, int _cx, int _cy, Align _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:

		inline void setButtonPressed(bool _pressed)
		{
			if (m_isStatePressed == _pressed) return;
			m_isStatePressed = _pressed;
			updateButtonState();
		}

		inline bool getButtonPressed() {return m_isStatePressed;}

	protected:

		virtual void _onMouseLostFocus(WidgetPtr _new);
		virtual void _onMouseSetFocus(WidgetPtr _old);
		virtual void _onMouseButtonPressed(bool _left);
		virtual void _onMouseButtonReleased(bool _left);

		inline void updateButtonState()
		{
			if (m_isFocus) {
				if (m_isPressed || m_isStatePressed) setState("select");
				else setState("active");
			} else {
				if (m_isPressed || m_isStatePressed) setState("pressed");
				else setState("normal");
			}
		}


	private:
		// нажата ли кнопка
		bool m_isPressed;
		// в фокусе ли кнопка
		bool m_isFocus;
		// статус кнопки нажата или нет
		bool m_isStatePressed;

	}; // class Button : public Widget

	typedef StrangeButton * StrangeButtonPtr;
}

#endif // __STRANGE_BUTTON_H__