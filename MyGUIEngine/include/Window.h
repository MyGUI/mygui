#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Widget.h"

namespace MyGUI
{

	class _MyGUIExport Window : public Widget, public Ogre::FrameListener
	{
		// для вызова закрытого конструктора
		friend WidgetFactory<Window>;

	protected:
		Window(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("Window"); return type;};

		void showWindowCaption(bool _show);
		void showWindowX(bool _show);
		void showWindowResize(bool _show);

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		void show(bool _show, bool _smoot);

	protected:

//		virtual void _onMouseLostFocus(WidgetPtr _new);
//		virtual void _onMouseSetFocus(WidgetPtr _old);
//		virtual void _onMouseButtonPressed(bool _left);
//		virtual void _onMouseButtonReleased(bool _left);

		virtual void _onMouseChangeRootFocus(bool _focus);
		virtual void _onKeyChangeRootFocus(bool _focus);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMousePressedX(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMovedCaption(MyGUI::WidgetPtr _sender, int _x, int _y);
		void notifyMouseMovedResize(MyGUI::WidgetPtr _sender, int _x, int _y);

		void setDoAlpha(float _alpha);

		// просто обновляет альфу взависимости от флагов
		void updateAlpha();

	private:
		std::string mSkinCaption, mSkinX, mSkinResize;
		WidgetPtr mWidgetCaption, mWidgetX, mWidgetResize;
		floatRect mOffsetCaption, mOffsetX, mOffsetResize;
		char mAlignCaption, mAlignX, mAlignResize;

		intSize mSkinSize;

		// размеры окна перед началом его изменений
		intRect m_preActionRect;

		// подписанны ли мы на фрейм старт
		bool m_bIsListenerAlpha;
		// альфа, которую мы пытаемся догнать
		float m_doAlpha;
		// уничтожить ли после скрытия
		bool m_isDestroy;

		// наши главные фокусы
		bool m_mouseRootFocus;
		bool m_keyRootFocus;


	}; // class Window : public Widget

	typedef Window * WindowPtr;

} // namespace MyGUI

#endif // _WINDOW_H_