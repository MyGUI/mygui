/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TEXT_EDIT_H__
#define __MYGUI_TEXT_EDIT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_SubWidgetTextInterface.h"
#include "MyGUI_TextEditOverlayElement.h"

namespace MyGUI
{

	class _MyGUIExport TextEdit : public SubWidgetTextInterface
	{

	public:
		TextEdit(const SubWidgetInfo &_info, const Ogre::String & _material, CroppedRectanglePtr _parent, size_t _id);
		virtual ~TextEdit();

		void show();
		void hide();

		void setTextAlign(Align _align);

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

		void setTextSelect(size_t _start, size_t _end);
		// возвращает положение курсора по произвольному положению
		size_t getTextCursorFromPoint(IntPoint & _point);
		// возвращает положение курсора по позиции
		IntPoint getTextCursorFromPosition(size_t _position);
		// возвращает размер текста в пикселях
		IntSize getTextSize();
		IntSize getTextSize(const Ogre::DisplayString& _text);
		// устанавливает смещение текста в пикселях
		void setTextShift(IntPoint _point);
		IntPoint getTextShift();

		bool getSelectBackground();
		void setSelectBackground(bool _normal);

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		void _updateView(); // обновления себя и детей
		inline void _updateText() // обновляем все что касаеться текста
		{
			// и делаем полное обновление и выравнивание
			mIsMargin = true; // при изменении размеров все пересчитывать
			_setAlign(mParent->getSize(), true);
		}

		inline static const Ogre::String & _getType() {static Ogre::String type("TextEdit"); return type;}
		inline static bool _isSharedOverlay() {return false;}
		bool _isText() {return true;}

		Ogre::OverlayElement* _getOverlayElement();

	protected:

		TextEditOverlayElement * mOverlayContainer;
		Ogre::ColourValue mColour;

	}; // class _MyGUIExport TextEdit : public SubWidgetTextInterface

} // namespace MyGUI

#endif // __MYGUI_TEXT_EDIT_H__
