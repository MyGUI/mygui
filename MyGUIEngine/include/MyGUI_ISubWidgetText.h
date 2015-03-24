/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_SUB_WIDGET_TEXT_H_
#define MYGUI_I_SUB_WIDGET_TEXT_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ISubWidget.h"
#include "MyGUI_Colour.h"
#include "MyGUI_Constants.h"

namespace MyGUI
{

	class MYGUI_EXPORT ISubWidgetText :
		public ISubWidget
	{
		MYGUI_RTTI_DERIVED( ISubWidgetText )

	public:
		virtual ~ISubWidgetText()  { }

		/** Get index of start of selection */
		virtual size_t getTextSelectionStart() const
		{
			return 0;
		}

		/** Get index of end of selection */
		virtual size_t getTextSelectionEnd() const
		{
			return 0;
		}

		/** Set text selection */
		virtual void setTextSelection(size_t _start, size_t _end) { }

		// интенсивность выделенного текста
		virtual bool getSelectBackground() const
		{
			return true;
		}

		virtual void setSelectBackground(bool _normal) { }

		// нужно ли инвертировать выделение
		virtual bool getInvertSelected() const
		{
			return true;
		}

		virtual void setInvertSelected(bool _value) { }

		// нужно ли показывать тень
		virtual bool getShadow() const
		{
			return false;
		}

		virtual void setShadow(bool _value) { }

		// управление видимостью курсора
		virtual bool isVisibleCursor() const
		{
			return false;
		}

		virtual void setVisibleCursor(bool _value) { }

		// управление положением курсора
		virtual size_t getCursorPosition() const
		{
			return 0;
		}

		virtual void setCursorPosition(size_t _index) { }

		virtual void setWordWrap(bool _value) { }

		// возвращает положение курсора по произвольному положению
		// позиция абсолютная, без учета смещений
		virtual size_t getCursorPosition(const IntPoint& _point) /*const*/
		{
			return 0;
		}

		// возвращает положение курсора в обсолютных координатах
		virtual IntCoord getCursorCoord(size_t _position) /*const*/
		{
			return IntCoord();
		}

		// возвращает положение курсора в обсолютных координатах
		IntPoint getCursorPoint(size_t _position) /*const*/
		{
			const IntCoord& coord = getCursorCoord(_position);
			return IntPoint(coord.left + coord.width / 2, coord.top + coord.height / 2);
		}

		// возвращает положение курсора в обсолютных координатах
		IntRect getCursorRect(size_t _position) /*const*/
		{
			const IntCoord& coord = getCursorCoord(_position);
			return IntRect(coord.left, coord.top, coord.left + coord.width, coord.top + coord.height);
		}

		// возвращает размер текста в пикселях
		virtual IntSize getTextSize() /*const*/
		{
			return IntSize();
		}

		// устанавливает смещение текста в пикселях
		virtual void setViewOffset(const IntPoint& _point) { }
		virtual IntPoint getViewOffset() const
		{
			return IntPoint();
		}

		virtual void setCaption(const UString& _value) { }
		virtual const UString& getCaption() const
		{
			return Constants::getEmptyUString();
		}

		virtual void setTextColour(const Colour& _value) { }
		virtual const Colour& getTextColour() const
		{
			return Colour::Zero;
		}

		virtual void setFontName(const std::string& _value) { }
		virtual const std::string& getFontName() const
		{
			return Constants::getEmptyString();
		}

		virtual void setFontHeight(int _value) { }
		virtual int getFontHeight() const
		{
			return 0;
		}

		virtual void setTextAlign(Align _value) { }
		virtual Align getTextAlign() const
		{
			return Align::Default;
		}

		virtual void setShiftText(bool _value) { }

		virtual void setShadowColour(const Colour& _value) { }
		virtual const Colour& getShadowColour() const
		{
			return Colour::Zero;
		}

	};

} // namespace MyGUI

#endif // MYGUI_I_SUB_WIDGET_TEXT_H_
