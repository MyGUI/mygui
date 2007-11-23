#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Prerequest.h"
#include "ButtonFactory.h"
#include "Widget.h"

namespace MyGUI
{

	class _MyGUIExport Button : public Widget
	{
		// для вызова закрытого конструктора
		friend factory::ButtonFactory;

	protected:
		Button(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

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

	typedef Button* ButtonPtr;

} // namespace MyGUI

#endif // __BUTTON_H__