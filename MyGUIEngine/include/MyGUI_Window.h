/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WINDOW_H__
#define __MYGUI_WINDOW_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WindowFactory.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class Window;
	typedef Window * WindowPtr;

	class _MyGUIExport Window : public Widget
	{
		// для вызова закрытого конструктора
		friend factory::WindowFactory;

	protected:
		Window(int _left, int _top, int _width, int _height, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);
		~Window();

		void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);

	public:
		// переопределяем для присвоению клиенту
		virtual WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name = "");

		// для мееедленного показа и скрытия
		void showSmooth(bool _reset = false);
		void hideSmooth(bool _destroy = false);

		inline bool getAutoAlpha() {return mIsAutoAlpha;}
		inline void setAutoAlpha(bool _auto) {mIsAutoAlpha=_auto; if(!_auto)setAlpha(1.0f);else updateAlpha();}

		virtual void setCaption(const Ogre::DisplayString & _caption) {if (mWidgetCaption!=null)mWidgetCaption->setCaption(_caption);};
		virtual const Ogre::DisplayString & getCaption() {if (mWidgetCaption!=null) return mWidgetCaption->getCaption();return Widget::getCaption();};

		void setMinMax(IntRect _minmax) {mMinmax = _minmax;}
		IntRect getMinMax() {return mMinmax;}

		virtual void setPosition(int _left, int _top);
		virtual void setPosition(int _left, int _top, int _width, int _height);
		virtual void setSize(int _width, int _height);

		inline bool getIsToStick() {return mIsToStick;}
		inline void setIsToStick(bool _stick) {mIsToStick = _stick;}

		virtual IntRect getClientRect()
		{
			if (null == mWidgetClient) return IntRect(mLeft, mTop, mLeft + mWidth, mTop + mHeight);
			return mWidgetClient->getClientRect();
		}

		// нажат крестик на окне
		EventSimple eventWindowXPressed;

	protected:
		void _frameStarted(float _frame, float _event);

		virtual void _onMouseChangeRootFocus(bool _focus);
		virtual void _onKeyChangeRootFocus(bool _focus);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMousePressedX(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMovedCaption(MyGUI::WidgetPtr _sender, int _left, int _top);
		void notifyMouseMovedResize(MyGUI::WidgetPtr _sender, int _left, int _top);

		void setDoAlpha(float _alpha);

		// просто обновляет альфу взависимости от флагов
		void updateAlpha();

	private:
		WidgetPtr mWidgetCaption, mWidgetX, mWidgetResize, mWidgetClient;

		// размеры окна перед началом его изменений
		IntRect mPreActionRect;

		// подписанны ли мы на фрейм старт
		bool mIsListenerAlpha;
		// альфа, которую мы пытаемся догнать
		float mDoAlpha;
		// уничтожить ли после скрытия
		bool mIsDestroy;

		// наши главные фокусы
		bool mMouseRootFocus;
		bool mKeyRootFocus;

		// автоматическое или ручное управление альфой
		bool mIsAutoAlpha;

		// минимальные и максимальные размеры окна
		IntRect mMinmax;

		bool mIsToStick; // прилеплять ли к краям


	}; // class Window : public Widget

} // namespace MyGUI

#endif // __MYGUI_WINDOW_H__