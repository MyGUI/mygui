/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_I_SUB_WIDGET_TEXT_H__
#define __MYGUI_I_SUB_WIDGET_TEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_ISubWidget.h"
#include "MyGUI_Colour.h"

namespace MyGUI
{

	class MYGUI_EXPORT ISubWidgetText : public ISubWidget
	{
		MYGUI_RTTI_CHILD_HEADER( ISubWidgetText, ISubWidget );

	public:
		ISubWidgetText(const IntCoord& _coord, Align _align, ICroppedRectangle * _parent) :
			ISubWidget(_coord, _align, _parent) { }
		virtual ~ISubWidgetText()  { }

		virtual bool firstQueue() { return false; }

		/** Get index of start of selection */
		virtual size_t getSelectStart() { return 0; }
		/** Get index of end of selection */
		virtual size_t getSelectEnd() { return 0; }
		/** Set text selection */
		virtual void setTextSelect(size_t _start, size_t _end) { }

		// интенсивность выделенного текста
		virtual bool getSelectBackground() { return true; }
		virtual void setSelectBackground(bool _normal) { }

		// управление видимостью курсора
		virtual bool isCursorShow() { return false; }
		virtual void setShowCursor(bool _show) { }

		// управление положением курсора
		virtual size_t getCursorPosition() { return 0; }
		virtual void setCursorPosition(size_t _pos) { }

		virtual void setBreakLine(bool _break) { }

		// возвращает положение курсора по произвольному положению
		virtual size_t getCursorPosition(const IntPoint & _point) { return 0; }

		// возвращает положение курсора в обсолютных координатах
		virtual IntCoord getCursorCoord(size_t _position) { return IntCoord(); }

		// возвращает положение курсора в обсолютных координатах
		IntPoint getCursorPoint(size_t _position)
		{
			const IntCoord & coord = getCursorCoord(_position);
			return IntPoint(coord.left + coord.width / 2, coord.top + coord.height / 2);
		}

		// возвращает положение курсора в обсолютных координатах
		IntRect getCursorRect(size_t _position)
		{
			const IntCoord & coord = getCursorCoord(_position);
			return IntRect(coord.left, coord.top, coord.left + coord.width, coord.top + coord.height);
		}

		// возвращает размер текста в пикселях
		virtual IntSize getTextSize() { return IntSize(); }

		// устанавливает смещение текста в пикселях
		virtual void setViewOffset(IntPoint _point) { }
		virtual IntPoint getViewOffset() { return IntPoint(); }

		virtual void setCaption(const Ogre::UTFString & _caption) { }
		virtual const Ogre::UTFString & getCaption() { static Ogre::UTFString caption; return caption; }

		virtual void setTextColour(const Colour& _colour) { }
		virtual const Colour& getTextColour() { return Colour::Zero; }

		virtual void setFontName(const Ogre::String & _font) { }
		virtual const Ogre::String & getFontName() { static Ogre::String name; return name; }

		virtual void setFontHeight(uint _height) { }
		virtual uint getFontHeight() { return 0; }

		virtual void setTextAlign(Align _align) { }
		virtual Align getTextAlign() { return Align::Default; }

		virtual void setShiftText(bool _shift) { }

	};

} // namespace MyGUI

#endif // __MYGUI_I_SUB_WIDGET_TEXT_H__
