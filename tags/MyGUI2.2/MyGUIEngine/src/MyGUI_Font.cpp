/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_Font.h"
#include "MyGUI_Common.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_EnumCharInfo.h"

#include <OgreTextureManager.h>
#include <OgreResourceGroupManager.h>
#include <OgreImage.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	const unsigned char FONT_MASK_SELECT = 0x88;
	const unsigned char FONT_MASK_SELECT_DEACTIVE = 0x60;
	const unsigned char FONT_MASK_SPACE = 0x00;
	const unsigned char FONT_MASK_CHAR = 0xFF;
	const size_t MIN_FONT_TEXTURE_WIDTH = 256;

    //---------------------------------------------------------------------
	Font::Font(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
		const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader) :
		Ogre::Resource (creator, name, handle, group, isManual, loader),
		mTtfSize(0),
		mTtfResolution(0),
		mDistance(0),
		mSpaceWidth(0),
		mTabWidth(0),
		mCursorWidth(0),
		mOffsetHeight(0),
		mDefaultHeight(0),
		mAntialiasColour(false)
    {
    }
    //---------------------------------------------------------------------
    Font::~Font()
    {
        // have to call this here reather than in Resource destructor
        // since calling virtual methods in base destructors causes crash
        unload();
    }
    //---------------------------------------------------------------------
	Font::GlyphInfo * Font::getGlyphInfo(Char _id)
	{
		for (VectorRangeInfo::iterator iter=mVectorRangeInfo.begin(); iter!=mVectorRangeInfo.end(); ++iter)
		{
			GlyphInfo * info = iter->getInfo(_id);
			if (info == nullptr) continue;
			return info;
		}
		// при ошибках возвращаем пробел
		return & mSpaceGlyphInfo;
	}
    //---------------------------------------------------------------------
	void Font::loadImpl()
	{
		if (isTrueType()) {
			// create texture
			Ogre::String texName = mName + "_Texture";
			// Create, setting isManual to true and passing self as loader
			mTexture = Ogre::TextureManager::getSingleton().create(texName, mGroup, true, this);
			mTexture->setTextureType(Ogre::TEX_TYPE_2D);
			mTexture->setNumMipmaps(0);
			mTexture->load();
		}
	}
    //---------------------------------------------------------------------
	void Font::unloadImpl()
	{
		if (isTrueType()) {
			// удаляем все созданные ресурсы
			Ogre::TextureManager::getSingleton().remove(mTexture->getName());
		}
	}
    //---------------------------------------------------------------------
	void Font::loadResource(Ogre::Resource* res)
	{
		if (isTrueType()) {
			loadResourceTrueType(res);

		}
		else {
			checkTexture();
		}

	}
    //---------------------------------------------------------------------
	void Font::checkTexture()
	{
		if (mTexture.isNull()) {
			Ogre::TextureManager* manager = Ogre::TextureManager::getSingletonPtr();

			if (false == manager->resourceExists(mSource)) {
				const std::string& group = ResourceManager::getInstance().getResourceGroup();
				if (!helper::isFileExist(mSource, group)) {
					MYGUI_LOG(Error, "Texture '" + mSource + "' not found, set default texture");
				}
				else {
					mTexture = manager->load(mSource, group, Ogre::TEX_TYPE_2D, 0);
				}
			}
			else {
				mTexture = manager->getByName(mSource);
			}
		}
	}
    //---------------------------------------------------------------------
	void Font::loadResourceTrueType(Ogre::Resource* res)
	{

		FT_Library ftLibrary;
		// Init freetype
		if( FT_Init_FreeType( &ftLibrary ) ) MYGUI_EXCEPT("Could not init FreeType library!");

		// Locate ttf file, load it pre-buffered into memory by wrapping the
		// original DataStream in a MemoryDataStream
		Ogre::DataStreamPtr dataStreamPtr =
		Ogre::ResourceGroupManager::getSingleton().openResource(
			mSource, mGroup, true, this);
		Ogre::MemoryDataStream ttfchunk(dataStreamPtr);

		// Load font
		FT_Face face;
		if ( FT_New_Memory_Face( ftLibrary, ttfchunk.getPtr(), (FT_Long)ttfchunk.size() , 0, &face ) )
			MYGUI_EXCEPT("Could not open font face!");

		// Convert our point size to freetype 26.6 fixed point format
		FT_F26Dot6 ftSize = (FT_F26Dot6)(mTtfSize * (1 << 6));
		if ( FT_Set_Char_Size( face, ftSize, 0, mTtfResolution, mTtfResolution ) )
			MYGUI_EXCEPT("Could not set char size!");

		int max_height = 0, max_bear = 0;

		int mSelectionWidth = mCursorWidth;

		int spec_len = mCursorWidth + mSelectionWidth + mSelectionWidth + mSpaceWidth + mTabWidth + (mDistance * 5);
		int len = mDistance + spec_len;
		int height = 0; // здесь используется как колличество строк

		size_t finalWidth = MIN_FONT_TEXTURE_WIDTH;
		// trying to guess necessary width for texture
		while (mTtfSize*mTtfResolution > finalWidth*6) finalWidth *= 2;

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
				if ( int(finalWidth - 1) < (len + advance + mDistance) ) { height ++; len = mDistance;}

			}
		}

		max_height >>= 6;
		max_bear >>= 6;

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

		MYGUI_LOG(Info, "Font '" << mName << "' using texture size " << finalWidth << " x " << finalHeight);
		MYGUI_LOG(Info, "Font '" << mName << "' using real height " << max_height << " pixels");
		mDefaultHeight = max_height;

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
		if ( int(finalWidth - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

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
		if ( int(finalWidth - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

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
		advance = mSelectionWidth;
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
		if ( int(finalWidth - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

		addGlyph(&mSelectGlyphInfo, FontCodeType::Selected, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
		len += (advance + mDistance);

		//------------------------------------------------------------------
		// создаем неактивное выделение
		//------------------------------------------------------------------
		advance = mSelectionWidth;

		// перевод на новую строку
		if ( int(finalWidth - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

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

		addGlyph(&mSelectDeactiveGlyphInfo, FontCodeType::SelectedBack, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
		len += (advance + mDistance);

		//------------------------------------------------------------------
		// создаем курсор
		//------------------------------------------------------------------
		advance = mCursorWidth;

		// перевод на новую строку
		if ( int(finalWidth - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

		for (int j = 0; j < max_height; j++ )
		{
			int row = j + (int)height;
			uint8* pDest = &imageData[(row * data_width) + len * pixel_bytes];
			for(int k = 0; k < advance; k++ )
			{
				*pDest++= (k&1) ? 0 : 0xFF;
				*pDest++= FONT_MASK_CHAR;
			}
		}

		addGlyph(&mCursorGlyphInfo, FontCodeType::Cursor, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
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
				if ( int(finalWidth - 1) < (len + face->glyph->bitmap.width + mDistance) ) { height += max_height + mDistance; len = mDistance; }

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

		// Добавляем спец символы в основной список
		// пробел можно не добавлять, его вернет по ошибке
		RangeInfo info(FontCodeType::Selected, FontCodeType::Tab);
		info.setInfo(FontCodeType::Selected, &mSelectGlyphInfo);
		info.setInfo(FontCodeType::SelectedBack, &mSelectDeactiveGlyphInfo);
		info.setInfo(FontCodeType::Cursor, &mCursorGlyphInfo);
		info.setInfo(FontCodeType::Tab, &mTabGlyphInfo);

		mVectorRangeInfo.push_back(info);
		

		// FIXME хз что сделать надо, старый вариант падает, а с этим по идее утечка (не могу проверить)
#if OGRE_VERSION < MYGUI_DEFINE_VERSION(1, 6, 0)
		Ogre::DataStreamPtr memStream( new Ogre::MemoryDataStream(imageData, data_size, true) );
#else
		Ogre::DataStreamPtr memStream( new Ogre::MemoryDataStream(imageData, data_size) );
#endif

		Ogre::Image img;
		img.loadRawData( memStream, finalWidth, finalHeight, Ogre::PF_BYTE_LA );

		Ogre::Texture* tex = static_cast<Ogre::Texture*>(res);
		// Call internal _loadImages, not loadImage since that's external and
		// will determine load status etc again, and this is a manual loader inside load()
		Ogre::ConstImagePtrList imagePtrs;
		imagePtrs.push_back(&img);
		tex->_loadImages( imagePtrs );


		FT_Done_FreeType(ftLibrary);

		// ManualResourceLoader implementation - load the texture
		/*FT_Library ftLibrary;
		// Init freetype
		if( FT_Init_FreeType( &ftLibrary ) ) MYGUI_EXCEPT("Could not init FreeType library!");

		// Locate ttf file, load it pre-buffered into memory by wrapping the
		// original DataStream in a MemoryDataStream
		Ogre::DataStreamPtr dataStreamPtr =
		Ogre::ResourceGroupManager::getSingleton().openResource(
			mSource, mGroup, true, this);
		Ogre::MemoryDataStream ttfchunk(dataStreamPtr);

		// Load font
		FT_Face face;
		if ( FT_New_Memory_Face( ftLibrary, ttfchunk.getPtr(), (FT_Long)ttfchunk.size() , 0, &face ) )
			MYGUI_EXCEPT("Could not open font face!");

		// Convert our point size to freetype 26.6 fixed point format
		FT_F26Dot6 ftSize = (FT_F26Dot6)(mTtfSize * (1 << 6));
		if ( FT_Set_Char_Size( face, ftSize, 0, mTtfResolution, mTtfResolution ) )
			MYGUI_EXCEPT("Could not set char size!");

		int max_height = 0, max_bear = 0;

		int len = mDistance;
		int height = 0; // здесь используется как колличество строк

		for (VectorRangeInfo::iterator iter=mVectorRangeInfo.begin(); iter!=mVectorRangeInfo.end(); ++iter) {
			for (Char index=iter->first; index<=iter->last; ++index) {

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

		// вычисляем ближайшую кратную 2
		size_t needHeight = 1;
		while (needHeight < finalHeight) needHeight <<= 1;
		finalHeight = needHeight;

		Ogre::Real textureAspect = (float)finalWidth / (float)finalHeight;

		const size_t pixel_bytes = 2;
		size_t data_width = finalWidth * pixel_bytes;
		size_t data_size = finalWidth * finalHeight * pixel_bytes;

		MYGUI_LOG(Info, "Font '" << mName << "' using texture size " << finalWidth << " x " << finalHeight);
		MYGUI_LOG(Info, "Font '" << mName << "' using real height " << max_height << " pixels");
		mHeightPix = max_height;

        Ogre::uchar* imageData = new Ogre::uchar[data_size];
		// Reset content (White, transparent)
        for (size_t i = 0; i < data_size; i += pixel_bytes) {
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

		for (int j = 0; j < max_height; j++ ) {
			int row = j + (int)height;
			Ogre::uchar* pDest = &imageData[(row * data_width) + len * pixel_bytes];
			for (int k = 0; k < advance; k++ ) {
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

		for (int j = 0; j < max_height; j++ ) {
			int row = j + (int)height;
			Ogre::uchar* pDest = &imageData[(row * data_width) + len * pixel_bytes];
			for (int k = 0; k < advance; k++ ) {
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
		for (int j = 0; j < max_height; j++ ) {
			int row = j + (int)height;
			Ogre::uchar* pDest = &imageData[(row * data_width) + len * pixel_bytes];
			for(int k = 0; k < advance; k++ ) {
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_SELECT;
			}
		}

		// перевод на новую строку
		if ( int(FONT_TEXTURE_WIDTH - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

		addGlyph(&mSelectGlyphInfo, FontCodeType::Selected, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
		len += (advance + mDistance);

		//------------------------------------------------------------------
		// создаем неактивное выделение
		//------------------------------------------------------------------
		advance = mCursorWidth;

		// перевод на новую строку
		if ( int(FONT_TEXTURE_WIDTH - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

		for (int j = 0; j < max_height; j++ ) {
			int row = j + (int)height;
			Ogre::uchar* pDest = &imageData[(row * data_width) + len * pixel_bytes];
			for(int k = 0; k < advance; k++ ) {
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_SELECT_DEACTIVE;
			}
		}

		addGlyph(&mSelectDeactiveGlyphInfo, FontCodeType::SelectedBack, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
		len += (advance + mDistance);

		//------------------------------------------------------------------
		// создаем курсор
		//------------------------------------------------------------------
		advance = mCursorWidth;

		// перевод на новую строку
		if ( int(FONT_TEXTURE_WIDTH - 1) < (len + advance + mDistance) ) { height += max_height + mDistance; len = mDistance; }

		for (int j = 0; j < max_height; j++ ) {
			int row = j + (int)height;
			Ogre::uchar* pDest = &imageData[(row * data_width) + len * pixel_bytes];
			for(int k = 0; k < advance; k++ ) {
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_CHAR;
			}
		}

		addGlyph(&mCursorGlyphInfo, FontCodeType::Cursor, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
		len += (advance + mDistance);

		//------------------------------------------------------------------
		// создаем все остальные символы
		//------------------------------------------------------------------
		FT_Error ftResult;
		for (VectorRangeInfo::iterator iter=mVectorRangeInfo.begin(); iter!=mVectorRangeInfo.end(); ++iter) {

			size_t pos = 0;
			for (Char index=iter->first; index<=iter->last; ++index, ++pos) {

				// сомвол рисовать не нада
				if (checkHidePointCode(index)) continue;

				GlyphInfo & info = iter->range.at(pos);

				ftResult = FT_Load_Char( face, index, FT_LOAD_RENDER );
				if (ftResult) {
					// problem loading this glyph, continue
					MYGUI_LOG(Warning, "cannot load character " << index << " in font " << mName);
					continue;
				}

				FT_Int advance = (face->glyph->advance.x >> 6 );
				unsigned char* buffer = face->glyph->bitmap.buffer;

				if (nullptr == buffer) {
					// Yuck, FT didn't detect this but generated a nullptr pointer!
					MYGUI_LOG(Warning, "Freetype returned nullptr for character " << index << " in font " << mName);
					continue;
				}

				int y_bearnig = max_bear - ( face->glyph->metrics.horiBearingY >> 6 );

				// перевод на новую строку
				if ( int(FONT_TEXTURE_WIDTH - 1) < (len + face->glyph->bitmap.width + mDistance) ) { height += max_height + mDistance; len = mDistance; }

				for(int j = 0; j < face->glyph->bitmap.rows; j++ ) {
					int row = j + (int)height + y_bearnig;
					Ogre::uchar* pDest = &imageData[(row * data_width) + (len + ( face->glyph->metrics.horiBearingX >> 6 )) * pixel_bytes];
					for(int k = 0; k < face->glyph->bitmap.width; k++ ) {
						if (mAntialiasColour) *pDest++= *buffer;
						else *pDest++= FONT_MASK_CHAR;
						*pDest++= *buffer;
						buffer++;
					}
				}

				addGlyph(&info, index, len, height, len + advance, height + max_height, finalWidth, finalHeight, textureAspect, mOffsetHeight);
				len += (advance + mDistance);

			}
		}

		// FIXME хз что сделать надо, старый вариант падает, а с этим по идее утечка (не могу проверить)
#if OGRE_VERSION < MYGUI_DEFINE_VERSION(1, 6, 0)
		Ogre::DataStreamPtr memStream( new Ogre::MemoryDataStream(imageData, data_size, true) );
#else
		Ogre::DataStreamPtr memStream( new Ogre::MemoryDataStream(imageData, data_size) );
#endif

		Ogre::Image img;
		img.loadRawData( memStream, finalWidth, finalHeight, Ogre::PF_BYTE_LA );

		Ogre::Texture* tex = static_cast<Ogre::Texture*>(res);
		// Call internal _loadImages, not loadImage since that's external and
		// will determine load status etc again, and this is a manual loader inside load()
		Ogre::ConstImagePtrList imagePtrs;
		imagePtrs.push_back(&img);
		tex->_loadImages( imagePtrs );


		FT_Done_FreeType(ftLibrary);*/
    }
	//-----------------------------------------------------------------------
	void Font::addGlyph(GlyphInfo * _info, Char _index, int _left, int _top, int _right, int _bottom, int _finalw, int _finalh, float _aspect, int _addHeight)
	{
		_info->codePoint = _index;
		_info->uvRect.left = (float)_left / (float)_finalw;  // u1
		_info->uvRect.top = (float)(_top + _addHeight) / (float)_finalh;  // v1
		_info->uvRect.right = (float)( _right ) / (float)_finalw; // u2
		_info->uvRect.bottom = ( _bottom + _addHeight ) / (float)_finalh; // v2
		//_info->aspectRatio = _aspect * (_info->uvRect.right - _info->uvRect.left)  / (_info->uvRect.bottom - _info->uvRect.top);
		_info->width = _right - _left;
	}

	void Font::addGlyph(Char _code, const IntCoord& _coord)
	{
		mVectorPairCodeCoord.push_back(PairCodeCoord(_code, _coord));
	}

	void Font::initialise()
	{
		if (isTrueType()) return;
		if (mVectorPairCodeCoord.empty()) return;

		std::sort(mVectorPairCodeCoord.begin(), mVectorPairCodeCoord.end());

		const IntSize& size = SkinManager::getTextureSize(mSource);
		float aspect = (float)size.width / (float)size.height;

		Char code = mVectorPairCodeCoord.front().code;
		size_t count = mVectorPairCodeCoord.size();
		size_t first = 0;

		for (size_t pos=1; pos<count; ++pos) {

			// диапазон оборвался
			if (code + 1 != mVectorPairCodeCoord[pos].code) {
				addRange(mVectorPairCodeCoord, first, pos-1, size.width, size.height, aspect);
				code = mVectorPairCodeCoord[pos].code;
				first = pos;
			}
			else {
				code ++;
			}

		}

		addRange(mVectorPairCodeCoord, first, count-1, size.width, size.height, aspect);

		// уничтожаем буфер
		VectorPairCodeCoord tmp;
		std::swap(tmp, mVectorPairCodeCoord);

		checkTexture();
	}

	void Font::addRange(VectorPairCodeCoord & _info, size_t _first, size_t _last, int _width, int _height, float _aspect)
	{
		RangeInfo range = RangeInfo(_info[_first].code, _info[_last].code);

		for (size_t pos=_first; pos<=_last; ++pos)
		{
			GlyphInfo* info = range.getInfo(_info[pos].code);
			const IntCoord& coord = _info[pos].coord;
			addGlyph(info, _info[pos].code, coord.left, coord.top, coord.right(), coord.bottom(), _width, _height, _aspect);

			if (_info[pos].code == FontCodeType::Space)
				mSpaceGlyphInfo = *info;
			/*else if (_info[pos].code == FontCodeType::Tab)
				mTabGlyphInfo = *info;
			else if (_info[pos].code == FontCodeType::Selected)
				mSelectGlyphInfo = *info;
			else if (_info[pos].code == FontCodeType::SelectedBack)
				mSelectDeactiveGlyphInfo = *info;
			else if (_info[pos].code == FontCodeType::Cursor)
				mCursorGlyphInfo = *info;*/
		}

		mVectorRangeInfo.push_back(range);
	}

} // namespace MyGUI
