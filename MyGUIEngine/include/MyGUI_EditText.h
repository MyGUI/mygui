/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*//*
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
#include "MyGUI_DrawItem.h"
#include "MyGUI_Font.h"
#include "MyGUI_EnumCharInfo.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	class RenderItem;

	class MYGUI_EXPORT EditText : public ISubWidgetText
	{
		MYGUI_RTTI_CHILD_HEADER(EditText, ISubWidgetText);

	public:
		EditText(const SubWidgetInfo &_info, ICroppedRectangle * _parent);
		virtual ~EditText();

		virtual void setVisible(bool _visible);

		// обновляет все данные связанные с тектом
		virtual void updateRawData();

		// метод для отрисовки себя
		virtual size_t _drawItem(Vertex * _vertex, bool _update);

		void _updateView();
		void _correctView();

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		void setCaption(const Ogre::UTFString & _caption);
		const Ogre::UTFString & getCaption();

		void setTextColour(const Colour& _colour);
		const Colour& getTextColour();

		void setAlpha(float _alpha);
		float getAlpha();

		virtual void setFontName(const std::string & _font);
		virtual const std::string & getFontName();

		virtual void setFontHeight(uint _height);
		virtual uint getFontHeight();

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

		void setShiftText(bool _shift);

		void setBreakLine(bool _break);

		virtual void _setStateData(StateInfo * _data);

		// метод для генерации данных из описания xml
		static StateInfo * createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version);

	protected:

		bool mEmptyView;
		uint32 mCurrentColour, mInverseColour;
		uint32 mCurrentAlpha;
		IntCoord mCurrentCoord;

		Ogre::UTFString mCaption;
		bool mTextOutDate;
		Align mTextAlign;

		Colour mColour;
		float mAlpha;
		bool mRenderGL;

		FontPtr mpFont;
		Ogre::TexturePtr mpTexture;
		uint mFontHeight;
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
		IntSize mContextSize; // размер всего текста

		LayerItemKeeper * mItemKeeper;
		RenderItem * mRenderItem;
		size_t mCountVertex;

		LayerManager * mManager;

		bool mManualView;
		bool mShiftText;
		bool mBreakLine;
		int mOldWidth;

	};

} // namespace MyGUI

#endif // __MYGUI_EDIT_TEXT_H__
