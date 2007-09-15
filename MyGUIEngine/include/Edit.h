#ifndef _EDIT_H_
#define _EDIT_H_

#include "Prerequest.h"
#include "Widget.h"

namespace MyGUI
{

	class _MyGUIExport Edit : public Widget
	{
		// для вызова закрытого конструктора
		friend WidgetFactory<Edit>;

	protected:
		Edit(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("Edit"); return type;};

	protected:

		virtual void OnMouseLostFocus(WidgetPtr _new);
		virtual void OnMouseSetFocus(WidgetPtr _old);
		virtual void OnMouseButtonPressed(bool _left);
		virtual void OnMouseButtonReleased(bool _left);
		virtual void OnKeyLostFocus(WidgetPtr _new);
		virtual void OnKeySetFocus(WidgetPtr _old);
		virtual void OnKeyButtonPressed(int _key, wchar_t _char);
		virtual void OnKeyButtonReleased(int _key);

		inline void updateEditState()
		{
			if (m_isFocus) {
				if (m_isPressed) setState("select");
				else setState("active");
			} else {
				if (m_isPressed) setState("pressed");
				else setState("normal");
			}
		}


	private:
		// нажата ли кнопка
		bool m_isPressed;
		// в фокусе ли кнопка
		bool m_isFocus;

	}; // class Edit : public Widget

	typedef Edit * EditPtr;

} // namespace MyGUI

#endif
