/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WINDOW_H__
#define __MYGUI_WINDOW_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_EventPair.h"

namespace MyGUI
{

	// OBSOLETE
	typedef delegates::CDelegate2<WidgetPtr, const std::string&> EventHandle_WidgetString;

	typedef delegates::CDelegate2<WindowPtr, const std::string&> EventHandle_WindowPtrCStringRef;
	typedef delegates::CDelegate1<WindowPtr> EventHandle_WindowPtr;

	class MYGUI_EXPORT Window : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<Window>;

		MYGUI_RTTI_CHILD_HEADER( Window, Widget );

	public:
		/** @copydoc Widget::setVisible */
		virtual void setVisible(bool _visible);
		/** Hide or show Menu smooth */
		void setVisibleSmooth(bool _visible);
		/** Hide window smooth and then destroy it */
		void destroySmooth();

		/** Get auto alpha mode flag */
		bool getAutoAlpha() {return mIsAutoAlpha;}
		/** Enable or disable auto alpha mode */
		void setAutoAlpha(bool _auto);

		/** Set window caption */
		virtual void setCaption(const Ogre::UTFString & _caption);
		/** Get window caption */
		virtual const Ogre::UTFString & getCaption();

		/** Set minimal possible window size */
		void setMinSize(const IntSize & _size) { mMinmax.left = _size.width; mMinmax.top = _size.height; }
		/** Set minimal possible window size */
		void setMinSize(int _width, int _height) { mMinmax.left = _width; mMinmax.top = _height; }
		/** Get minimal possible window size */
		IntSize getMinSize() { return IntSize(mMinmax.left, mMinmax.top); }

		/** Set maximal possible window size */
		void setMaxSize(const IntSize & _size) { mMinmax.right = _size.width; mMinmax.bottom = _size.height; }
		/** Set maximal possible window size */
		void setMaxSize(int _width, int _height) { mMinmax.right = _width; mMinmax.bottom = _height; }
		/** Get maximal possible window size */
		IntSize getMaxSize() { return IntSize(mMinmax.right, mMinmax.bottom); }

		//! @copydoc Widget::setPosition(const IntPoint & _point)
		virtual void setPosition(const IntPoint & _point);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize & _size);
		//! @copydoc Widget::setCoord(const IntCoord & _coord)
		virtual void setCoord(const IntCoord & _coord);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		/** Get snap to borders mode flag */
		bool getSnap() { return mSnap; }
		/** Enable or disable snap to borders mode */
		void setSnap(bool _snap) { mSnap = _snap; }

		//! @copydoc Widget::setTextAlign
		virtual void setTextAlign(Align _align);
		//! @copydoc Widget::getTextAlign
		virtual Align getTextAlign();

		//! @copydoc Widget::setTextColour
		virtual void setTextColour(const Colour& _colour);
		//! @copydoc Widget::getTextColour
		virtual const Colour& getTextColour();

		//! @copydoc Widget::setFontName
		virtual void setFontName(const Ogre::String & _font);
		//! @copydoc Widget::getFontName
		virtual const Ogre::String & getFontName();

		//! @copydoc Widget::setFontHeight
		virtual void setFontHeight(uint _height);
		//! @copydoc Widget::getFontHeight
		virtual uint getFontHeight();


	/*event:*/
		/** Event : Window button pressed.\n
			signature : void method(MyGUI::WindowPtr _sender, const std::string& _name)
			@param _sender widget that called this event
			@param _name of pressed button
		*/
		EventPair<EventHandle_WidgetString, EventHandle_WindowPtrCStringRef> eventWindowButtonPressed;

		/** Event : Window coordinate changed (window was resized or moved).\n
			signature : void method(MyGUI::WindowPtr _sender)
			@param _sender widget that called this event
		*/
		EventPair<EventHandle_WidgetVoid, EventHandle_WindowPtr> eventWindowChangeCoord;


	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord & _coord) { setCoord(_coord); }
		MYGUI_OBSOLETE("use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height) { setCoord(_left, _top, _width, _height); }
		MYGUI_OBSOLETE("use : void setVisibleSmooth(bool _visible)")
		void showSmooth(bool _reset = false) { setVisibleSmooth(true); }
		MYGUI_OBSOLETE("use : void setVisibleSmooth(bool _visible)")
		void hideSmooth() { setVisibleSmooth(false); }
		MYGUI_OBSOLETE("use : void setMinSize(const IntSize& _min) , void setMaxSize(const IntSize& _min)")
		void setMinMax(const IntRect & _minmax) { setMinSize(_minmax.left, _minmax.top); setMaxSize(_minmax.right, _minmax.bottom); }
		MYGUI_OBSOLETE("use : void setMinSize(const IntSize& _min) , void setMaxSize(const IntSize& _min)")
		void setMinMax(int _min_w, int _min_h, int _max_w, int _max_h) { setMinSize(_min_w, _min_h); setMaxSize(_max_w, _max_h); }
		MYGUI_OBSOLETE("use : IntSize getMinSize() , IntSize getMaxSize()")
		IntRect getMinMax() { return IntRect(getMinSize().width, getMinSize().height, getMaxSize().width, getMaxSize().height); }

#endif // MYGUI_DONT_USE_OBSOLETE

	protected:
		Window(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~Window();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

		// переопределяем для присвоению клиенту
		virtual WidgetPtr baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		void onMouseChangeRootFocus(bool _focus);
		void onKeyChangeRootFocus(bool _focus);
		void onMouseDrag(int _left, int _top);
		void onMouseButtonPressed(int _left, int _top, MouseButton _id);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyPressedButtonEvent(MyGUI::WidgetPtr _sender);
		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);

		// просто обновляет альфу взависимости от флагов
		void updateAlpha();

		void animateStop(WidgetPtr _widget);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

		float getAlphaVisible();

	private:
		WidgetPtr mWidgetCaption;

		// размеры окна перед началом его изменений
		IntCoord mPreActionCoord;

		// наши главные фокусы
		bool mMouseRootFocus;
		bool mKeyRootFocus;

		// автоматическое или ручное управление альфой
		bool mIsAutoAlpha;

		// минимальные и максимальные размеры окна
		IntRect mMinmax;

		bool mSnap; // прилеплять ли к краям

		IntCoord mCurrentActionScale;
		bool mAnimateSmooth;

	};

} // namespace MyGUI

#endif // __MYGUI_WINDOW_H__
