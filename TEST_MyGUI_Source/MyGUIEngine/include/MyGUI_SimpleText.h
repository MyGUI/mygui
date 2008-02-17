/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_SIMPLE_TEXT_H__
#define __MYGUI_SIMPLE_TEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_SubWidgetTextInterface.h"
#include "MyGUI_DrawItem.h"
#include "MyGUI_Font.h"
#include "MyGUI_EnumCharInfo.h"

#include <OgreMaterial.h>

namespace MyGUI
{

	class RenderItem;

	class _MyGUIExport SimpleText : public SubWidgetTextInterface, public DrawItem
	{

	public:
		SimpleText(const SubWidgetInfo &_info, CroppedRectanglePtr _parent);
		virtual ~SimpleText();

		inline static const Ogre::String & _getType() {static Ogre::String type("SimpleText"); return type;}

		void show();
		void hide();

		void setTextAlign(Align _align);

		void _updateView();
		void _correctView();

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		void setCaption(const Ogre::DisplayString & _caption);
		const Ogre::DisplayString & getCaption();

		void setColour(const Ogre::ColourValue & _colour);
		const Ogre::ColourValue & getColour();

		void setAlpha(float _alpha);
		float getAlpha();

		void setFontName(const std::string & _font);
		const std::string & getFontName();
		void setFontHeight(uint16 _height);
		uint16 getFontHeight();
		inline void setFontName(const std::string & _font, uint16 _height)
		{
			setFontName(_font);
			setFontHeight(_height);
		}

		IntSize getTextSize();
		IntSize getTextSize(const Ogre::DisplayString& _text);

		void updateRawData();

		virtual void _createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item);
		virtual void _destroyDrawItem();

		// метод для отрисовки себя
		virtual size_t _drawItem(Vertex * _vertex);


	protected:

		bool mTransparent;
		uint32 mCurrentColour;
		IntCoord mCurrentCoord;

		Ogre::DisplayString mCaption;
		bool mTextOutDate;
		Align mTextAlign;

		Ogre::ColourValue mColour;
		float mAlpha;
		bool mRenderGL;

		FontPtr mpFont;
		Ogre::TexturePtr mpTexture;
		uint16 mFontHeight;
		Font::GlyphInfo * mSpaceGlyphInfo;
		Font::GlyphInfo * mTabGlyphInfo;
		float mAspectCoef;

		float mTextureHeightOne, mTextureWidthOne;
		FloatPoint mBackgroundEmpty, mBackgroundFill, mBackgroundFillDeactive;

		VectorLineInfo mLinesInfo;
		IntPoint mPointShift; // смещение текста
		FloatSize mContextRealSize; // размер всего текста

		LayerItemKeeper * mItemKeeper;
		RenderItem * mRenderItem;
		size_t mCountVertex;

	};

} // namespace MyGUI

#endif // __MYGUI_SIMPLE_TEXT_H__
