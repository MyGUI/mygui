#ifndef _TEXTEDIT_H_
#define _TEXTEDIT_H_

#include "Prerequest.h"
#include "BasisWidget.h"
#include "TextEditOverlayElement.h"

namespace MyGUI
{
	using namespace Ogre;


	class _MyGUIExport TextEdit : public BasisWidget
	{

	public:
		TextEdit(const tagBasisWidgetInfo &_info, const String & _material, BasisWidgetPtr _parent);
		virtual ~TextEdit();

		void show(bool _show);

		void setCaption(const Ogre::DisplayString & _caption);
		const Ogre::DisplayString & getCaption();

		void setColour(const Ogre::ColourValue & _color);
		const Ogre::ColourValue & getColour() {return m_color;}

		void setAlpha(float _alpha);
		float getAlpha() {return m_color.a;};

		void setFontName(const Ogre::String & _font);
		void setFontName(const Ogre::String & _font, Ogre::ushort _height);
		const Ogre::String & getFontName();

		void setFontHeight(Ogre::ushort _height);
		Ogre::ushort getFontHeight();

		void setTextAlign(char _align);

		void setTextSelect(size_t _start, size_t _end);
		// ���������� ��������� ������� �� ������������� ���������
		size_t getTextCursorFromPoint(IntPoint & _point);
		// ���������� ��������� ������� �� �������
		IntPoint getTextCursorFromPosition(size_t _position);
		// ���������� ������ ������ � ��������
		IntSize getTextSize();
		// ������������� �������� ������ � ��������
		void setTextShift(IntPoint _point);
		IntPoint getTextShift();

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void update(); // ���������� ���� � �����
		inline void updateText() // ��������� ��� ��� ��������� ������
		{
			// ��������� �����
			//m_overlayContainer->updateText();
			// � ������ ������ ���������� � ������������
			m_margin = true; // ��� ��������� �������� ��� �������������
			align(m_parent->width(), m_parent->height(), true);
		}

		bool isText() {return true;};

		inline const static Ogre::String & getType() {static Ogre::String type("TextEdit"); return type;};

		OverlayElementPtr getOverlayElement();

	protected:

		TextEditOverlayElement * m_overlayContainer;
		Ogre::ColourValue m_color;

	}; // class TextEdit : public BasisWidget

} // namespace MyGUI

#endif
