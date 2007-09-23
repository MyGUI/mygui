#ifndef _VSCROLL_H_
#define _VSCROLL_H_

#include "Widget.h"

namespace MyGUI
{

	class _MyGUIExport VScroll : public Widget
	{
		// для вызова закрытого конструктора
		friend WidgetFactory<VScroll>;

	protected:
		VScroll(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("VScroll"); return type;};

/*		inline void setButtonPressed(bool _pressed)
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
		bool m_isStatePressed;*/
	private:
		// наши кнопки
		WidgetPtr m_widgetTop;
		WidgetPtr m_widgetBottom;
		WidgetPtr m_widgetTrack;

	}; // class VScroll : public Widget

	typedef VScroll * VScrollPtr;

} // namespace MyGUI


#endif // _VSCROLL_H_