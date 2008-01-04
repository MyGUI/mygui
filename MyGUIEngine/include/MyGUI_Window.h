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

	typedef delegates::CDelegate2<WidgetPtr, const std::string&> EventButtonPressedInfo;

	class _MyGUIExport Window : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::WindowFactory;

	protected:
		Window(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);
		~Window();

	public:
		// переопределяем для присвоению клиенту
		virtual WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name = "");
		// тип данного виджета
		inline static const Ogre::String & getType() {static Ogre::String type("Window"); return type;}

		// для мееедленного показа и скрытия
		void showSmooth(bool _reset = false);
		void hideSmooth();
		void destroySmooth();

		inline bool getAutoAlpha() {return mIsAutoAlpha;}
		inline void setAutoAlpha(bool _auto)
		{
			mIsAutoAlpha = _auto;
			if (false == _auto) setAlpha(ALPHA_MAX);
			else updateAlpha();
		}

		virtual void setCaption(const Ogre::DisplayString & _caption) {if (mWidgetCaption!=null)mWidgetCaption->setCaption(_caption);};
		virtual const Ogre::DisplayString & getCaption() {if (mWidgetCaption!=null) return mWidgetCaption->getCaption();return Widget::getCaption();};

		void setMinMax(IntRect _minmax) {mMinmax = _minmax;}
		IntRect getMinMax() {return mMinmax;}

		virtual void setPosition(const IntPoint& _pos);
		virtual void setPosition(const IntCoord& _coord);
		virtual void setSize(const IntSize& _size);

		inline bool getIsToStick() {return mIsToStick;}
		inline void setIsToStick(bool _stick) {mIsToStick = _stick;}

		const IntCoord& getClientRect();

		virtual VectorWidgetPtr getChilds();

		// event : нажат на кнопку в окне
		// signature : void method(MyGUI::WidgetPtr _widget, const std::string& _name)
		EventButtonPressedInfo eventWindowButtonPressed;

	protected:
		void _frameEntered(float _frame);

		void _onMouseChangeRootFocus(bool _focus);
		void _onKeyChangeRootFocus(bool _focus);
		void _onMouseMove(int _left, int _top);
		void _onMouseButtonPressed(bool _left);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyPressedButtonEvent(MyGUI::WidgetPtr _sender, bool _double);
		void notifyMouseMovedAction(MyGUI::WidgetPtr _sender, int _left, int _top);

		void setDoAlpha(float _alpha);

		// просто обновляет альфу взависимости от флагов
		void updateAlpha();

	private:
		WidgetPtr mWidgetCaption, mWidgetClient;

		// размеры окна перед началом его изменений
		IntCoord mPreActionCoord;

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

		IntCoord mCurrentActionScale;


	}; // class Window : public Widget

} // namespace MyGUI

#endif // __MYGUI_WINDOW_H__
