#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Widget.h"

namespace MyGUI
{

	class Button : public Widget
	{
		// для вызова закрытого конструктора
		friend WidgetFactory<Button>;

	protected:
		Button(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("Button"); return type;};

		inline void setButtonPressed(bool _pressed)
		{
			if (m_isStatePressed == _pressed) return;
			m_isStatePressed = _pressed;
			updateButtonState();
		}

		inline bool getButtonPressed() {return m_isStatePressed;}

	protected:

		virtual void OnMouseLostFocus(WidgetPtr _new);
		virtual void OnMouseSetFocus(WidgetPtr _old);
		virtual void OnMouseButtonPressed(bool _left);
		virtual void OnMouseButtonReleased(bool _left);

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

	typedef Button * ButtonPtr;

} // namespace MyGUI


#endif