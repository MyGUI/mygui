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

		// ��� ����� ��������
		bool _isText() {return true;}

		// ������ � ���������� �������
		virtual size_t getSelectStart() {return 0;}
		virtual size_t getSelectEnd() {return 0;}
		virtual void setTextSelect(size_t _start, size_t _end) {}

		// ������������� ����������� ������
		virtual bool getSelectBackground() {return true;}
		virtual void setSelectBackground(bool _normal) {}

		// ���������� ���������� �������
		virtual bool isCursorShow() {return false;}
		virtual void setShowCursor(bool _show) {}

		// ���������� ���������� �������
		virtual size_t getCursorPosition() {return 0;}
		virtual void setCursorPosition(size_t _pos) {}

		// ���������� ��������� ������� �� ������������� ���������
		virtual size_t getCursorPosition(const IntPoint & _point) {return 0;}

		// ���������� ��������� ������� � ���������� �����������
		virtual IntCoord getCursorCoord(size_t _position) {return IntCoord();}

		// ���������� ��������� ������� � ���������� �����������
		inline IntPoint getCursorPoint(size_t _position)
		{
			const IntCoord & coord = getCursorCoord(_position);
			return IntPoint(coord.left + coord.width / 2, coord.top + coord.height / 2);
		}

		// ���������� ��������� ������� � ���������� �����������
		inline IntRect getCursorRect(size_t _position)
		{
			const IntCoord & coord = getCursorCoord(_position);
			return IntRect(coord.left, coord.top, coord.left + coord.width, coord.top + coord.height);
		}

		// ���������� ������ ������ � ��������
		virtual IntSize getTextSize() {return IntSize();}

		// ������������� �������� ������ � ��������
		virtual void setViewOffset(IntPoint _point) {}
		virtual IntPoint getViewOffset() {return IntPoint();}

		virtual void setCaption(const Ogre::UTFString & _caption) {}
		virtual const Ogre::UTFString & getCaption() {static Ogre::UTFString caption; return caption;}

		virtual void setColour(const Ogre::ColourValue & _colour) {}
		virtual const Ogre::ColourValue & getColour() {return Ogre::ColourValue::Black;}

		virtual void setFontName(const Ogre::String & _font) {}
		virtual const Ogre::String & getFontName() {static Ogre::String name; return name;}

		virtual void setFontHeight(uint16 _height) {}
		virtual uint16 getFontHeight() {return 0;}

		virtual void setTextAlign(Align _align) {}
		virtual Align getTextAlign() { return ALIGN_DEFAULT; }

	};

} // namespace MyGUI

#endif // __MYGUI_SUB_WIDGET_TEXT_INTERFACE_H__
