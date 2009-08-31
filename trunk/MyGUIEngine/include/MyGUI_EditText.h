/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_EDIT_TEXT_H__
#define __MYGUI_EDIT_TEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_IFont.h"
#include "MyGUI_EnumCharInfo.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{

	class RenderItem;

	class MYGUI_EXPORT EditText : public ISubWidgetText
	{
		MYGUI_RTTI_DERIVED( EditText );

	public:
		EditText();
		virtual ~EditText();

		virtual void setVisible(bool _visible);

		// обновляет все данные связанные с тектом
		virtual void updateRawData();

		// метод для отрисовки себя
		virtual void doRender();

		void setCaption(const UString& _value);
		const UString& getCaption();

		void setTextColour(const Colour& _value);
		const Colour& getTextColour();

		void setAlpha(float _alpha);
		float getAlpha();

		virtual void setFontName(const std::string& _value);
		virtual const std::string& getFontName();

		virtual void setFontHeight(int _value);
		virtual int getFontHeight();

		virtual void createDrawItem(const std::string& _texture, ILayerNode * _node);
		virtual void destroyDrawItem();

		virtual void setTextAlign(Align _align);
		virtual Align getTextAlign();

		virtual size_t getTextSelectionStart();
		virtual size_t getTextSelectionEnd();
		virtual void setTextSelection(size_t _start, size_t _end);

		virtual bool getSelectBackground();
		virtual void setSelectBackground(bool _normal);

		virtual bool isVisibleCursor();
		virtual void setVisibleCursor(bool _value);

		virtual size_t getCursorPosition();
		virtual void setCursorPosition(size_t _index);

		virtual IntSize getTextSize();

		// устанавливает смещение текста в пикселях
		virtual void setViewOffset(const IntPoint& _point);
		virtual IntPoint getViewOffset();

		// возвращает положение курсора по произвольному положению
		virtual size_t getCursorPosition(const IntPoint& _point);

		// возвращает положение курсора в обсолютных координатах
		virtual IntCoord getCursorCoord(size_t _position);

		void setShiftText(bool _shift);

		void setWordWrap(bool _value);

		virtual void setStateData(IStateInfo * _data);

		void _updateView();
		void _correctView();

	/*internal:*/
		void _setAlign(const IntSize& _oldsize, bool _update);
		void _setAlign(const IntCoord& _oldcoord, bool _update);

	protected:
		bool mEmptyView;
		uint32 mCurrentColour, mInverseColour;
		uint32 mCurrentAlpha;
		IntCoord mCurrentCoord;

		UString mCaption;
		bool mTextOutDate;
		Align mTextAlign;

		Colour mColour;
		float mAlpha;
		VertexColourType mVertexFormat;

		IFont* mFont;
		ITexture* mTexture;
		int mFontHeight;
		GlyphInfo * mSpaceGlyphInfo;
		GlyphInfo * mTabGlyphInfo;

		float mTextureHeightOne, mTextureWidthOne;
		bool mBackgroundNormal;
		size_t mStartSelect, mEndSelect;
		size_t mCursorPosition;
		bool mShowCursor;
		FloatPoint mBackgroundEmpty, mBackgroundFill, mBackgroundFillDeactive, mCursorTexture;

		VectorLineInfo mLinesInfo;
		IntPoint mViewOffset; // смещение текста
		FloatSize mContextRealSize; // размер всего текста
		IntSize mContextSize; // размер всего текста

		ILayerNode* mNode;
		RenderItem* mRenderItem;
		size_t mCountVertex;

		bool mManualView;
		bool mShiftText;
		bool mBreakLine;
		int mOldWidth;
	};

} // namespace MyGUI

#endif // __MYGUI_EDIT_TEXT_H__
