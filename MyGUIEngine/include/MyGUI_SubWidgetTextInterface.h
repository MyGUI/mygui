/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_SUB_WIDGET_TEXT_INTERFACE_H__
#define __MYGUI_SUB_WIDGET_TEXT_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_CroppedRectangleInterface.h"

namespace MyGUI
{

	class _MyGUIExport SubWidgetTextInterface : public CroppedRectangleInterface
	{
	public:
		SubWidgetTextInterface(const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent) :
			CroppedRectangleInterface(_coord, _align, _parent)
		{}

		bool _isText() {return true;}

		virtual void setTextSelect(size_t _start, size_t _end) {}
		// возвращает положение курсора по произвольному положению
		virtual size_t getTextCursorFromPoint(IntPoint & _point) {return 0;}
		// возвращает положение курсора по позиции
		virtual IntPoint getTextCursorFromPosition(size_t _position) {return IntPoint();}
		// возвращает размер текста в пикселях
		virtual IntSize getTextSize() {return IntSize();}
		virtual IntSize getTextSize(const Ogre::DisplayString& _text) {return IntSize();}

		// устанавливает смещение текста в пикселях
		virtual void setTextShift(IntPoint _point) {}
		virtual IntPoint getTextShift() {return IntPoint();}

		virtual bool getSelectBackground() {return true;}
		virtual void setSelectBackground(bool _normal) {}

		virtual void setCaption(const Ogre::DisplayString & _caption) {}
		virtual const Ogre::DisplayString & getCaption() {static Ogre::DisplayString caption; return caption;}

		virtual void setColour(const Ogre::ColourValue & _colour) {}
		virtual const Ogre::ColourValue & getColour() {return Ogre::ColourValue::Black;}

		virtual void setFontName(const Ogre::String & _font) {}
		virtual void setFontName(const Ogre::String & _font, Ogre::ushort _height) {}
		virtual const Ogre::String & getFontName() {static Ogre::String name; return name;}

		virtual void setFontHeight(Ogre::ushort _height) {}
		virtual Ogre::ushort getFontHeight() {return 0;}

		virtual void setTextAlign(Align _align) {}
		inline Align getAlign() {return mAlign;}

	}; // class _MyGUIExport SubWidgetTextInterface : public CroppedRectangleInterface

} // namespace MyGUI

#endif // __MYGUI_SUB_WIDGET_TEXT_INTERFACE_H__
