#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Widget.h"

namespace MyGUI
{

	class _MyGUIExport Window : public Widget, public Ogre::FrameListener
	{
		// дл€ вызова закрытого конструктора
		friend WidgetFactory<Window>;

	protected:
		Window(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		// переопредел€ем дл€ присвоению клиенту
		virtual WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _name = "");

		inline const static Ogre::String & getType() {static Ogre::String type("Window"); return type;};

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		void show(bool _smoot = true, bool _reset = true);
		void hide(bool _smoot = true, bool _destroy = false);

		inline bool getAutoAlpha() {return m_bIsAutoAlpha;}
		inline void setAutoAlpha(bool _auto) {m_bIsAutoAlpha=_auto; if(!_auto)setAlpha(1.0f);else updateAlpha();}

		virtual void setCaption(const Ogre::DisplayString & _caption) {if (mWidgetCaption!=null)mWidgetCaption->setCaption(_caption);};
		virtual const Ogre::DisplayString & getCaption() {if (mWidgetCaption!=null) return mWidgetCaption->getCaption();return Widget::getCaption();};

		void setMinMax(IntRect _minmax) {m_minmax = _minmax;}
		IntRect getMinMax() {return m_minmax;}

		virtual void move(int _x, int _y);
		virtual void move(int _x, int _y, int _cx, int _cy);
		virtual void size(int _cx, int _cy);

		inline bool getIsToStick() {return mIsToStick;}
		inline void setIsToStick(bool _stick) {mIsToStick = _stick;}

		// нажат крестик на окне
		EventSimple eventWindowXPressed;

	protected:

		virtual void _onMouseChangeRootFocus(bool _focus);
		virtual void _onKeyChangeRootFocus(bool _focus);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMousePressedX(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMovedCaption(MyGUI::WidgetPtr _sender, int _x, int _y);
		void notifyMouseMovedResize(MyGUI::WidgetPtr _sender, int _x, int _y);

		void setDoAlpha(float _alpha);

		// просто обновл€ет альфу взависимости от флагов
		void updateAlpha();

	private:
		WidgetPtr mWidgetCaption, mWidgetX, mWidgetResize, mWidgetClient;

		// размеры окна перед началом его изменений
		IntRect m_preActionRect;

		// подписанны ли мы на фрейм старт
		bool m_bIsListenerAlpha;
		// альфа, которую мы пытаемс€ догнать
		float m_doAlpha;
		// уничтожить ли после скрыти€
		bool m_isDestroy;

		// наши главные фокусы
		bool m_mouseRootFocus;
		bool m_keyRootFocus;

		// автоматическое или ручное управление альфой
		bool m_bIsAutoAlpha;

		// минимальные и максимальные размеры окна
		IntRect m_minmax;

		bool mIsToStick; // прилепл€ть ли к кра€м


	}; // class Window : public Widget

	typedef Window * WindowPtr;

} // namespace MyGUI

#endif // _WINDOW_H_