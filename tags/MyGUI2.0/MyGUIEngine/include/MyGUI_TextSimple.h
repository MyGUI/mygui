/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TEXT_SIMPLE_H__
#define __MYGUI_TEXT_SIMPLE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_SubWidgetTextInterface.h"
#include "MyGUI_TextSimpleOverlayElement.h"

namespace MyGUI
{

	class _MyGUIExport TextSimple : public SubWidgetTextInterface
	{

	public:
		TextSimple(const SubWidgetInfo &_info, const Ogre::String & _material, CroppedRectanglePtr _parent, size_t _id);
		virtual ~TextSimple();

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

		IntSize getTextSize();
		IntSize getTextSize(const Ogre::DisplayString& _text);

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		void _updateView(); // обновления себя и детей
		inline void _updateText() // обновляем все что касаеться текста
		{
			// и делаем полное обновление и выравнивание
			mIsMargin = true; // при изменении размеров все пересчитывать
			_setAlign(mParent->getSize(), true);
		}

		inline const static Ogre::String & _getType() {static Ogre::String type("TextSimple"); return type;}
		inline static bool _isSharedOverlay() {return false;}
		bool _isText() {return true;}

		Ogre::OverlayElement* _getOverlayElement();

	protected:

		TextSimpleOverlayElement * mOverlayContainer;
		Ogre::ColourValue mColour;

	}; // class _MyGUIExport TextSimple : public SubWidgetTextInterface

} // namespace MyGUI

#endif // __MYGUI_TEXT_SIMPLE_H__
