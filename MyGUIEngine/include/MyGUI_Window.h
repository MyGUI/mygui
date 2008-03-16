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

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {static Ogre::String type("Window"); return type;}
		//!	@copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		// переопределяем для присвоению клиенту
		virtual WidgetPtr _createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		// для мееедленного показа и скрытия
		void showSmooth(bool _reset = false);
		void hideSmooth();
		void destroySmooth();

		inline bool getAutoAlpha() {return mIsAutoAlpha;}
		void setAutoAlpha(bool _auto);

		virtual void setCaption(const Ogre::DisplayString & _caption);
		virtual const Ogre::DisplayString & getCaption();

		inline void setMinMax(IntRect _minmax) {mMinmax = _minmax;}
		inline void setMinMax(int _min_h, int _min_v, int _max_h, int _max_v) {mMinmax.set(_min_h, _min_v, _max_h, _max_v);}
		inline IntRect getMinMax() {return mMinmax;}

		virtual void setPosition(const IntPoint& _pos);
		virtual void setPosition(const IntCoord& _coord);
		virtual void setSize(const IntSize& _size);

		inline void setPosition(int _left, int _top) {setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		inline bool getSnap() {return mSnap;}
		inline void setSnap(bool _snap) {mSnap = _snap;}

		virtual void setTextAlign(Align _align);
		virtual Align getTextAlign();

		virtual void setColour(const Ogre::ColourValue & _colour);
		virtual const Ogre::ColourValue & getColour();

		virtual void setFontName(const Ogre::String & _font);
		virtual const Ogre::String & getFontName();

		virtual void setFontHeight(uint16 _height);
		virtual uint16 getFontHeight();

		const IntCoord& getClientRect();

		virtual VectorWidgetPtr getChilds();

		// event : нажат на кнопку в окне
		// signature : void method(MyGUI::WidgetPtr _sender, const std::string& _name)
		EventInfo_WidgetString eventWindowButtonPressed;

		// event : изменились размеры окна
		// signature : void method(MyGUI::WidgetPtr _sender)
		EventInfo_WidgetVoid eventWindowChangeCoord;

	protected:

		void _onMouseChangeRootFocus(bool _focus);
		void _onKeyChangeRootFocus(bool _focus);
		void _onMouseDrag(int _left, int _top);
		void _onMouseButtonPressed(bool _left);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyPressedButtonEvent(MyGUI::WidgetPtr _sender);
		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);

		// просто обновляет альфу взависимости от флагов
		void updateAlpha();

	private:
		WidgetPtr mWidgetCaption, mWidgetClient;

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
