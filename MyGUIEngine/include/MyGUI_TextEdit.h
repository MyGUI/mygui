/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TEXT_EDIT_H__
#define __MYGUI_TEXT_EDIT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_BasisWidget.h"
#include "MyGUI_TextEditOverlayElement.h"

namespace MyGUI
{

	class _MyGUIExport TextEdit : public BasisWidget
	{

	public:
		TextEdit(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidgetPtr _parent);
		virtual ~TextEdit();

		void show(bool _show);

		void setCaption(const Ogre::DisplayString & _caption);
		const Ogre::DisplayString & getCaption();

		void setColour(const Ogre::ColourValue & _colour);
		const Ogre::ColourValue & getColour() {return mColour;}

		void setAlpha(float _alpha);
		float getAlpha() {return mColour.a;};

		void setFontName(const Ogre::String & _font);
		void setFontName(const Ogre::String & _font, Ogre::ushort _height);
		const Ogre::String & getFontName();

		void setFontHeight(Ogre::ushort _height);
		Ogre::ushort getFontHeight();

		void setTextAlign(Align _align);

		void setTextSelect(size_t _start, size_t _end);
		// возвращает положение курсора по произвольному положению
		size_t getTextCursorFromPoint(IntPoint & _point);
		// возвращает положение курсора по позиции
		IntPoint getTextCursorFromPosition(size_t _position);
		// возвращает размер текста в пикселях
		IntSize getTextSize();
		// устанавливает смещение текста в пикселях
		void setTextShift(IntPoint _point);
		IntPoint getTextShift();

		void align(int _width, int _height, bool _update);
		void align(int _left, int _top, int _width, int _height, bool _update);

		void update(); // обновления себя и детей
		inline void updateText() // обновляем все что касаеться текста
		{
			// изменился текст
			//mOverlayContainer->updateText();
			// и делаем полное обновление и выравнивание
			mMargin = true; // при изменении размеров все пересчитывать
			align(mParent->getWidth(), mParent->getHeight(), true);
		}

		bool isText() {return true;}

		inline static const Ogre::String & getType() {static Ogre::String type("TextEdit"); return type;}

		Ogre::OverlayElement* getOverlayElement();

	protected:

		TextEditOverlayElement * mOverlayContainer;
		Ogre::ColourValue mColour;

	}; // class TextEdit : public BasisWidget

} // namespace MyGUI

#endif // __MYGUI_TEXT_EDIT_H__