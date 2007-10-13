#ifndef _EDIT_H_
#define _EDIT_H_

#include "Prerequest.h"
#include "Widget.h"

namespace MyGUI
{

	class _MyGUIExport Edit : public Widget, public Ogre::FrameListener
	{
		// для вызова закрытого конструктора
		friend WidgetFactory<Edit>;

	protected:
		Edit(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("Edit"); return type;};

	protected:

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);

		virtual void _onMouseLostFocus(WidgetPtr _new);
		virtual void _onMouseSetFocus(WidgetPtr _old);
		virtual void _onKeyLostFocus(WidgetPtr _new);
		virtual void _onKeySetFocus(WidgetPtr _old);
		virtual void _onKeyButtonPressed(int _key, wchar_t _char);
		virtual void _onKeyButtonReleased(int _key);

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

		WidgetPtr mWidgetUpper;
		WidgetPtr mWidgetCursor;

		bool mCursorActive;
		float mTimer;

	}; // class Edit : public Widget

	typedef Edit * EditPtr;

} // namespace MyGUI

#endif
