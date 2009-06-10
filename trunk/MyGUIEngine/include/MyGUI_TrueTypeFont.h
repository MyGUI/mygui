/*!
	@file
	@author		Albert Semenov
	@date		06/2009
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
#ifndef __MYGUI_TRUE_TYPE_FONT_H__
#define __MYGUI_TRUE_TYPE_FONT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_IManualResourceLoader.h"
#include "MyGUI_IFont.h"

namespace MyGUI
{

	class MYGUI_EXPORT TrueTypeFont :
		public IFont,
		public IManualResourceLoader
    {
		MYGUI_RTTI_DERIVED( TrueTypeFont );

	public:
		typedef std::vector<PairCodePoint> VectorPairCodePoint;
		typedef std::vector<RangeInfo> VectorRangeInfo;
		typedef std::vector<PairCodeCoord> VectorPairCodeCoord;

	public:
		TrueTypeFont();
        virtual ~TrueTypeFont();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		virtual GlyphInfo* getSpaceGlyphInfo() { return & mSpaceGlyphInfo; }
		virtual GlyphInfo* getTabGlyphInfo() { return & mTabGlyphInfo; }
		virtual GlyphInfo* getSelectGlyphInfo() { return & mSelectGlyphInfo; }
		virtual GlyphInfo* getSelectDeactiveGlyphInfo() { return & mSelectDeactiveGlyphInfo; }
		virtual GlyphInfo* getCursorGlyphInfo() { return & mCursorGlyphInfo; }

		virtual GlyphInfo* getGlyphInfo(Char _id);

		virtual ITexture* getTextureFont() { return mTexture; }
		virtual const std::string& getName() { return mName; }

		// получившаяся высота при генерации в пикселях
		virtual int getDefaultHeight() { return mHeightPix; }

	private:
		void addCodePointRange(Char _first, Char _second);
		void addHideCodePointRange(Char _first, Char _second);

		// проверяет, входит ли символ в зоны ненужных символов
		bool checkHidePointCode(Char _id);

		/** Clear the list of code point ranges. */
		void clearCodePointRanges();

		void initialise();

		void addGlyph(GlyphInfo * _info, Char _index, int _left, int _top, int _right, int _bottom, int _finalw, int _finalh, float _aspect, int _addHeight = 0);

		virtual void loadResource(IRenderResource* _resource);

	private:
        // Source of the font
		std::string mSource;
        // Size of the truetype font, in points
		float mTtfSize;
        // Resolution (dpi) of truetype font
		uint mTtfResolution;

		bool mAntialiasColour;

		int mDistance;
		int mSpaceWidth;
		int mTabWidth;
		int mCursorWidth;
		int mOffsetHeight;
		int mHeightPix;

		// отдельная информация о символах
		GlyphInfo mSpaceGlyphInfo, mTabGlyphInfo, mSelectGlyphInfo, mSelectDeactiveGlyphInfo, mCursorGlyphInfo;

		// символы которые не нужно рисовать
		VectorPairCodePoint mVectorHideCodePoint;

		// вся информация о символах
		VectorRangeInfo mVectorRangeInfo;

		MyGUI::ITexture* mTexture;

		std::string mName;
    };

} // namespace MyGUI

#endif // __MYGUI_TRUE_TYPE_FONT_H__
