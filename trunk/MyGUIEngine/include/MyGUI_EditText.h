/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_EDIT_TEXT_H__
#define __MYGUI_EDIT_TEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_SubWidgetTextInterface.h"
#include "MyGUI_DrawItem.h"
#include "MyGUI_Font.h"
#include "MyGUI_EnumCharInfo.h"

namespace MyGUI
{

	class RenderItem;

	class _MyGUIExport EditText : public SubWidgetTextInterface, public DrawItem
	{

	public:
		EditText(const SubWidgetInfo &_info, CroppedRectanglePtr _parent);
		virtual ~EditText();

		void show();
		void hide();

		// обновляет все данные связанные с тектом
		void updateRawData();

		// метод для отрисовки себя
		virtual size_t _drawItem(Vertex * _vertex, bool _update);

		void _updateView();
		void _correctView();

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		void setCaption(const Ogre::UTFString & _caption);
		const Ogre::UTFString & getCaption();

		void setColour(const Ogre::ColourValue & _colour);
		const Ogre::ColourValue & getColour();

		void setAlpha(float _alpha);
		float getAlpha();

		virtual void setFontName(const std::string & _font);
		virtual const std::string & getFontName();

		virtual void setFontHeight(uint16 _height);
		virtual uint16 getFontHeight();

		virtual void _createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item);
		virtual void _destroyDrawItem();

		virtual void setTextAlign(Align _align);
		virtual Align getTextAlign();

		virtual size_t getSelectStart();
		virtual size_t getSelectEnd();
		virtual void setTextSelect(size_t _start, size_t _end);

		virtual bool getSelectBackground();
		virtual void setSelectBackground(bool _normal);

		virtual bool isCursorShow();
		virtual void setShowCursor(bool _show);

		virtual size_t getCursorPosition();
		virtual void setCursorPosition(size_t _pos);

		virtual IntSize getTextSize();

		// устанавливает смещение текста в пикселях
		virtual void setViewOffset(IntPoint _point);
		virtual IntPoint getViewOffset();

		// возвращает положение курсора по произвольному положению
		virtual size_t getCursorPosition(const IntPoint & _point);

		// возвращает положение курсора в обсолютных координатах
		virtual IntCoord getCursorCoord(size_t _position);

	protected:

		bool mEmptyView;
		uint32 mCurrentColour, mInverseColour;
		uint32 mCurrentAlpha;
		IntCoord mCurrentCoord;

		Ogre::UTFString mCaption;
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

		float mTextureHeightOne, mTextureWidthOne;
		bool mBackgroundNormal;
		size_t mStartSelect, mEndSelect;
		size_t mCursorPosition;
		bool mShowCursor;
		FloatPoint mBackgroundEmpty, mBackgroundFill, mBackgroundFillDeactive, mCursorTexture;

		VectorLineInfo mLinesInfo;
		IntPoint mViewOffset; // смещение текста
		FloatSize mContextRealSize; // размер всего текста
		FloatSize mContextSize; // размер всего текста

		LayerItemKeeper * mItemKeeper;
		RenderItem * mRenderItem;
		size_t mCountVertex;

		LayerManager * mManager;

		bool mManualView;

	};

} // namespace MyGUI

#endif // __MYGUI_EDIT_TEXT_H__
