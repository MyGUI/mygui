/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_TrueTypeFont.h"
#include "MyGUI_Common.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_TextureManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace MyGUI
{

	const unsigned char FONT_MASK_SELECT = 0x88;
	const unsigned char FONT_MASK_SELECT_DEACTIVE = 0x60;
	const unsigned char FONT_MASK_SPACE = 0x00;
	const unsigned char FONT_MASK_CHAR = 0xFF;
	const size_t FONT_TEXTURE_WIDTH = 512;

	TrueTypeFont::TrueTypeFont() :
		mTtfSize(0),
		mTtfResolution(0),
		mDistance(0),
		mSpaceWidth(0),
		mTabWidth(0),
		mCursorWidth(0),
		mOffsetHeight(0),
		mHeightPix(0),
		mTexture(nullptr),
		mAntialiasColour(false)
	{
	}

	TrueTypeFont::~TrueTypeFont()
	{
		if (mTexture != nullptr)
		{
			TextureManager::getInstance().destroyTexture(mTexture);
			mTexture = nullptr;
		}
	}

	GlyphInfo * TrueTypeFont::getGlyphInfo(Char _id)
	{
		for (VectorRangeInfo::iterator iter=mVectorRangeInfo.begin(); iter!=mVectorRangeInfo.end(); ++iter)
		{
			GlyphInfo * info = iter->getInfo(_id);
			if (info == nullptr) continue;
			return info;
		}
		// при ошибках возвращаем пробел
		return &mSpaceGlyphInfo;
	}

	void TrueTypeFont::addGlyph(GlyphInfo * _info, Char _index, int _left, int _top, int _right, int _bottom, int _finalw, int _finalh, float _aspect, int _addHeight)
	{
		_info->codePoint = _index;
		_info->uvRect.left = (float)_left / (float)_finalw;  // u1
		_info->uvRect.top = (float)(_top + _addHeight) / (float)_finalh;  // v1
		_info->uvRect.right = (float)( _right ) / (float)_finalw; // u2
		_info->uvRect.bottom = ( _bottom + _addHeight ) / (float)_finalh; // v2
		_info->aspectRatio = _aspect * (_info->uvRect.right - _info->uvRect.left)  / (_info->uvRect.bottom - _info->uvRect.top);
	}

	void TrueTypeFont::initialise()
	{
		mTexture = TextureManager::getInstance().createTexture(MyGUI::utility::toString((int)this, "_TrueTypeFont"));
		mTexture->setManualResourceLoader(this);
		mTexture->create();
	}

	void TrueTypeFont::loadResource(IRenderResource* _resource)
	{
		mTexture = static_cast<ITexture*>(_resource);

		// ManualResourceLoader implementation - load the texture
		FT_Library ftLibrary;
		// Init freetype
		if( FT_Init_FreeType( &ftLibrary ) ) MYGUI_EXCEPT("Could not init FreeType library!");

		// Load font
		FT_Face face;

		//FIXME научить работать без шрифтов
		Data* data = DataManager::getInstance().getData(mSource/*, ResourceManager::getInstance().getResourceGroup()*/);
		MYGUI_ASSERT(data, "Could not open font face!");

		if ( FT_New_Memory_Face( ftLibrary, data->getData(), (FT_Long)data->getSize(), 0, &face ) )
			MYGUI_EXCEPT("Could not open font face!");

		// Convert our point size to freetype 26.6 fixed point format
		FT_F26Dot6 ftSize = (FT_F26Dot6)(mTtfSize * (1 << 6));
		if ( FT_Set_Char_Size( face, ftSize, 0, mTtfResolution, mTtfResolution ) )
			MYGUI_EXCEPT("Could not set char size!");

		int max_height = 0, max_bear = 0;

		int len = mDistance;
		int height = 0; // здесь используется как колличество строк

		for (VectorRangeInfo::iterator iter=mVectorRangeInfo.begin(); iter!=mVectorRangeInfo.end(); ++iter)
		{
			for (Char index=iter->first; index<=iter->last; ++index)
			{

				// символ рисовать ненужно
				if (checkHidePointCode(index)) continue;

				if (FT_Load_Char( face, index, FT_LOAD_RENDER )) continue;
				if (nullptr == face->glyph->bitmap.buffer) continue;
				FT_Int advance = (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 );

				if ( ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY ) > max_height )
					max_height = ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY );

				if ( face->glyph->metrics.horiBearingY > max_bear )
					max_bear = face->glyph->metrics.horiBearingY;

				len += (advance + mDistance);
				if ( int(FONT_TEXTURE_WIDTH - 1) < (len + advance + mDistance) ) { height ++; len = mDistance;}

			}
		}

		max_height >>= 6;
		max_bear >>= 6;

		// ширина служебных равна высоте шрифта
		len += (max_height + mDistance) * 5;

		size_t finalWidth = FONT_TEXTURE_WIDTH;
		size_t finalHeight = (height+1) * (max_height + mDistance) + mDistance;

		//make it more squared
		while (finalHeight > finalWidth)
		{
			finalHeight /= 2;
			finalWidth *= 2;
		}

		// вычисляем ближайшую кратную 2
		size_t needHeight = 1;
		while (needHeight < finalHeight) needHeight <<= 1;
		finalHeight = needHeight;

		float textureAspect = (float)finalWidth / (float)finalHeight;

		const size_t pixel_bytes = 2;
		size_t data_width = finalWidth * pixel_bytes;
		size_t data_size = finalWidth * finalHeight * pixel_bytes;

		MYGUI_LOG(Info, "TrueTypeFont '" << mName << "' using texture size " << finalWidth << " x " << finalHeight);
		MYGUI_LOG(Info, "TrueTypeFont '" << mName << "' using real height " << max_height << " pixels");
		mHeightPix = max_height;

        uint8* imageData = new uint8[data_size];
		// Reset content (White, transparent)
        for (size_t i = 0; i < data_size; i += pixel_bytes)
		{
            imageData[i + 0] = 0xFF; // luminance
            imageData[i + 1] = 0x00; // alpha
        }

		// текущее положение в текстуре
		len = mDistance;
		height = mDistance; // здесь используется как текущее положение высоты
		FT_Int advance = 0;

		//------------------------------------------------------------------
		// создаем символ пробела
		//------------------------------------------------------------------
		advance = mSpaceWidth;

		// перевод на новую строку
		if ( int(FONT_TEXTURE_WIDTH - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

		for (int j = 0; j < max_height; j++ )
		{
			int row = j + (int)height;
			uint8* pDest = &imageData[(row * data_width) + len * pixel_bytes];
			for (int k = 0; k < advance; k++ )
			{
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_SPACE;
			}
		}

		addGlyph(&mSpaceGlyphInfo, FontCodeType::Space, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
		len += (advance + mDistance);

		//------------------------------------------------------------------
		// создаем табуляцию
		//------------------------------------------------------------------
		advance = mTabWidth;

		// перевод на новую строку
		if ( int(FONT_TEXTURE_WIDTH - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

		for (int j = 0; j < max_height; j++ )
		{
			int row = j + (int)height;
			uint8* pDest = &imageData[(row * data_width) + len * pixel_bytes];
			for (int k = 0; k < advance; k++ )
			{
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_SPACE;
			}
		}

		addGlyph(&mTabGlyphInfo, FontCodeType::Tab, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
		len += (advance + mDistance);

		//------------------------------------------------------------------
		// создаем выделение
		//------------------------------------------------------------------
		advance = mCursorWidth;
		for (int j = 0; j < max_height; j++ )
		{
			int row = j + (int)height;
			uint8* pDest = &imageData[(row * data_width) + len * pixel_bytes];
			for(int k = 0; k < advance; k++ )
			{
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_SELECT;
			}
		}

		// перевод на новую строку
		if ( int(FONT_TEXTURE_WIDTH - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

		addGlyph(&mSelectGlyphInfo, FontCodeType::Select, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
		len += (advance + mDistance);

		//------------------------------------------------------------------
		// создаем неактивное выделение
		//------------------------------------------------------------------
		advance = mCursorWidth;

		// перевод на новую строку
		if ( int(FONT_TEXTURE_WIDTH - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

		for (int j = 0; j < max_height; j++ )
		{
			int row = j + (int)height;
			uint8* pDest = &imageData[(row * data_width) + len * pixel_bytes];
			for(int k = 0; k < advance; k++ )
			{
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_SELECT_DEACTIVE;
			}
		}

		addGlyph(&mSelectDeactiveGlyphInfo, FontCodeType::Select, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
		len += (advance + mDistance);

		//------------------------------------------------------------------
		// создаем курсор
		//------------------------------------------------------------------
		advance = mCursorWidth;

		// перевод на новую строку
		if ( int(FONT_TEXTURE_WIDTH - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

		for (int j = 0; j < max_height; j++ )
		{
			int row = j + (int)height;
			uint8* pDest = &imageData[(row * data_width) + len * pixel_bytes];
			for(int k = 0; k < advance; k++ )
			{
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_CHAR;
			}
		}

		addGlyph(&mCursorGlyphInfo, FontCodeType::Select, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
		len += (advance + mDistance);

		//------------------------------------------------------------------
		// создаем все остальные символы
		//------------------------------------------------------------------
		FT_Error ftResult;
		for (VectorRangeInfo::iterator iter=mVectorRangeInfo.begin(); iter!=mVectorRangeInfo.end(); ++iter)
		{
			size_t pos = 0;
			for (Char index=iter->first; index<=iter->last; ++index, ++pos)
			{
				// сомвол рисовать не нада
				if (checkHidePointCode(index)) continue;

				GlyphInfo& info = iter->range.at(pos);

				ftResult = FT_Load_Char( face, index, FT_LOAD_RENDER );
				if (ftResult)
				{
					// problem loading this glyph, continue
					MYGUI_LOG(Warning, "cannot load character " << index << " in font " << mName);
					continue;
				}

				FT_Int glyph_advance = (face->glyph->advance.x >> 6 );
				unsigned char* buffer = face->glyph->bitmap.buffer;

				if (nullptr == buffer)
				{
					// Yuck, FT didn't detect this but generated a nullptr pointer!
					MYGUI_LOG(Warning, "Freetype returned nullptr for character " << index << " in font " << mName);
					continue;
				}

				int y_bearnig = max_bear - ( face->glyph->metrics.horiBearingY >> 6 );

				// перевод на новую строку
				if ( int(FONT_TEXTURE_WIDTH - 1) < (len + face->glyph->bitmap.width + mDistance) ) { height += max_height + mDistance; len = mDistance; }

				for (int j = 0; j < face->glyph->bitmap.rows; j++ )
				{
					int row = j + (int)height + y_bearnig;
					uint8* pDest = &imageData[(row * data_width) + (len + ( face->glyph->metrics.horiBearingX >> 6 )) * pixel_bytes];
					for (int k = 0; k < face->glyph->bitmap.width; k++ )
					{
						if (mAntialiasColour) *pDest++= *buffer;
						else *pDest++= FONT_MASK_CHAR;
						*pDest++= *buffer;
						buffer++;
					}
				}

				addGlyph(&info, index, len, height, len + glyph_advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
				len += (glyph_advance + mDistance);

			}
		}

		mTexture->loadFromMemory(imageData, finalWidth, finalHeight, PixelFormat::L8A8);

		delete[] imageData;
		delete data;

		FT_Done_FreeType(ftLibrary);
	}

	void TrueTypeFont::addCodePointRange(Char _first, Char _second)
	{
		mVectorRangeInfo.push_back(RangeInfo(_first, _second));
	}

	void TrueTypeFont::addHideCodePointRange(Char _first, Char _second)
	{
		mVectorHideCodePoint.push_back(PairCodePoint(_first, _second));
	}

	// проверяет, входит ли символ в зоны ненужных символов
	bool TrueTypeFont::checkHidePointCode(Char _id)
	{
		for (VectorPairCodePoint::iterator iter=mVectorHideCodePoint.begin(); iter!=mVectorHideCodePoint.end(); ++iter)
		{
			if (iter->isExist(_id)) return true;
		}
		return false;
	}

	void TrueTypeFont::clearCodePointRanges()
	{
		mVectorRangeInfo.clear();
		mVectorHideCodePoint.clear();
	}

	void TrueTypeFont::deserialization(xml::ElementPtr _node, Version _version)
	{
		mName = _node->findAttribute("name");
		mSource = _node->findAttribute("source");
		mTtfSize = utility::parseFloat(_node->findAttribute("size"));
		mTtfResolution = utility::parseUInt(_node->findAttribute("resolution"));
		mAntialiasColour = utility::parseBool(_node->findAttribute("antialias_colour"));
		mSpaceWidth = utility::parseInt(_node->findAttribute("space_width"));
		mTabWidth = utility::parseInt(_node->findAttribute("tab_width"));
		mCursorWidth = utility::parseInt(_node->findAttribute("cursor_width"));
		mDistance = utility::parseInt(_node->findAttribute("distance"));
		mOffsetHeight = utility::parseInt(_node->findAttribute("offset_height"));

		xml::ElementEnumerator range = _node->getElementEnumerator();
		while (range.next("Code"))
		{
			std::string range_value;
			std::vector<std::string> parse_range;
			// диапазон включений
			if (range->findAttribute("range", range_value))
			{
				parse_range = utility::split(range_value);
				if (!parse_range.empty())
				{
					int first = utility::parseInt(parse_range[0]);
					int last = parse_range.size() > 1 ? utility::parseInt(parse_range[1]) : first;
					addCodePointRange(first, last);
				}
			}
			// диапазон исключений
			else if (range->findAttribute("hide", range_value))
			{
				parse_range = utility::split(range_value);
				if (!parse_range.empty())
				{
					int first = utility::parseInt(parse_range[0]);
					int last = parse_range.size() > 1 ? utility::parseInt(parse_range[1]) : first;
					addHideCodePointRange(first, last);
				}
			}
		}

		// инициализируем
		initialise();
	}

} // namespace MyGUI
