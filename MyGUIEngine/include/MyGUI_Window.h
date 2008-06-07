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

	class _MyGUIExport Window : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::WindowFactory;

	protected:
		Window(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		// переопределяем для присвоению клиенту
		virtual WidgetPtr _createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		// для мееедленного показа и скрытия
      /** Show window smooth */
		void showSmooth(bool _reset = false);
      /** Hide window smooth */
		void hideSmooth();
      /** Hide window smooth and then destroy it */
		void destroySmooth();

      /** Get auto alpha mode flag */
		inline bool getAutoAlpha() {return mIsAutoAlpha;}
      /** Enable or disable auto alpha mode */
		void setAutoAlpha(bool _auto);

      /** Set window caption */
		virtual void setCaption(const Ogre::UTFString & _caption);
      /** Get window caption */
		virtual const Ogre::UTFString & getCaption();

		/** Set minimal and maximal possible window size
			@param _minmax First two values - min width and height, second - max width and height
		*/
		inline void setMinMax(IntRect _minmax) {mMinmax = _minmax;}
		/** Set minimal and maximal possible window size
			@param First two values - min width and height, second - max width and height
		*/
		inline void setMinMax(int _min_h, int _min_v, int _max_h, int _max_v) {mMinmax.set(_min_h, _min_v, _max_h, _max_v);}
      /** Get minimal and maximal possible window size */
		inline IntRect getMinMax() {return mMinmax;}

      //! @copydoc Widget::setPosition(const IntPoint& _pos)
		virtual void setPosition(const IntPoint& _pos);
      //! @copydoc Widget::setPosition(const IntCoord& _coord)
		virtual void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		inline void setPosition(int _left, int _top) {setPosition(IntPoint(_left, _top));}
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		//! @copydoc Widget::setSize(int _width, int _height)
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

      /** Get snap to borders mode flag */
		inline bool getSnap() {return mSnap;}
      /** Enable or disable snap to borders mode */
		inline void setSnap(bool _snap) {mSnap = _snap;}

		//! @copydoc Widget::setTextAlign
		virtual void setTextAlign(Align _align);
		//! @copydoc Widget::getTextAlign
		virtual Align getTextAlign();

		//! @copydoc Widget::setColour
		virtual void setColour(const Ogre::ColourValue & _colour);
		//! @copydoc Widget::setColour
		virtual const Ogre::ColourValue & getColour();

		//! @copydoc Widget::setFontName
		virtual void setFontName(const Ogre::String & _font);
		//! @copydoc Widget::getFontName
		virtual const Ogre::String & getFontName();

		//! @copydoc Widget::setFontHeight
		virtual void setFontHeight(uint16 _height);
		//! @copydoc Widget::getFontHeight
		virtual uint16 getFontHeight();

		//! @copydoc Widget::getChilds
		virtual VectorWidgetPtr getChilds();

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

		void _onMouseChangeRootFocus(bool _focus);
		void _onKeyChangeRootFocus(bool _focus);
		void _onMouseDrag(int _left, int _top);
		void _onMouseButtonPressed(int _left, int _top, MouseButton _id);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyPressedButtonEvent(MyGUI::WidgetPtr _sender);
		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);

		// просто обновляет альфу взависимости от флагов
		void updateAlpha();

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
