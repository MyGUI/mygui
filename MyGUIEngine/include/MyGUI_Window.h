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

namespace MyGUI
{

	typedef delegates::CDelegate2<WidgetPtr, const std::string&> EventInfo_WidgetString;

	class MYGUI_EXPORT Window : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<Window>;

		MYGUI_RTTI_CHILD_HEADER( Window, Widget );

	public:

		// для мееедленного показа и скрытия
		/** Show window smooth */
		void showSmooth(bool _reset = false);
		/** Hide window smooth */
		void hideSmooth();
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

		/** Set minimal and maximal possible window size
			@param _minmax First two values - min width and height, second - max width and height
		*/
		void setMinMax(const IntRect & _minmax) { mMinmax = _minmax; }
		/** Set minimal and maximal possible window size
			@param First two values - min width and height, second - max width and height
		*/
		void setMinMax(int _min_h, int _min_v, int _max_h, int _max_v) { mMinmax.set(_min_h, _min_v, _max_h, _max_v); }
		/** Get minimal and maximal possible window size */
		const IntRect & getMinMax() {return mMinmax;}

		void setMinSize(const IntSize & _size) { mMinmax.left = _size.width; mMinmax.top = _size.height; }
		void setMinSize(int _width, int _height) { mMinmax.left = _width; mMinmax.top = _height; }
		IntSize getMinSize() { return IntSize(mMinmax.left, mMinmax.top); }

		void setMaxSize(const IntSize & _size) { mMinmax.right = _size.width; mMinmax.bottom = _size.height; }
		void setMaxSize(int _width, int _height) { mMinmax.right = _width; mMinmax.bottom = _height; }
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

		MYGUI_OBSOLETE("use Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord & _coord) { setCoord(_coord); }
		MYGUI_OBSOLETE("use Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height) { setCoord(_left, _top, _width, _height); }

		/** Get snap to borders mode flag */
		bool getSnap() {return mSnap;}
		/** Enable or disable snap to borders mode */
		void setSnap(bool _snap) {mSnap = _snap;}

		//! @copydoc Widget::setTextAlign
		virtual void setTextAlign(Align _align);
		//! @copydoc Widget::getTextAlign
		virtual Align getTextAlign();

		//! @copydoc Widget::setColour
		virtual void setColour(const Colour& _colour);
		//! @copydoc Widget::getColour
		virtual const Colour& getColour();

		//! @copydoc Widget::setFontName
		virtual void setFontName(const Ogre::String & _font);
		//! @copydoc Widget::getFontName
		virtual const Ogre::String & getFontName();

		//! @copydoc Widget::setFontHeight
		virtual void setFontHeight(uint16 _height);
		//! @copydoc Widget::getFontHeight
		virtual uint16 getFontHeight();

		/** Event : Window button pressed.\n
			signature : void method(MyGUI::WidgetPtr _sender, const std::string& _name)
			@param _name of pressed button
		*/
		EventInfo_WidgetString eventWindowButtonPressed;

		/** Event : Window coordinate changed (window was resized or moved).\n
			signature : void method(MyGUI::WidgetPtr _sender)
			@param _name of pressed button
		*/
		EventInfo_WidgetVoid eventWindowChangeCoord;

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

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();
		void actionWidgetHide(WidgetPtr _widget);

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

	}; // class Window : public Widget

} // namespace MyGUI

#endif // __MYGUI_WINDOW_H__
