/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include <OgreStableHeaders.h>

#include "MyGUI_Font.h"
#include "MyGUI_Common.h"
#include <OgreMaterialManager.h>
#include <OgreTextureManager.h>
#include <OgreTexture.h>
#include <OgreResourceGroupManager.h>
#include <OgreStringConverter.h>
#include <OgreRenderWindow.h>
#include <OgreException.h>
#include <OgreBlendMode.h>
#include <OgreTextureUnitState.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreMaterial.h>
#include <OgreBitwise.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace MyGUI
{

	const unsigned char FONT_MASK_SELECT = 0x88;
	const unsigned char FONT_MASK_SELECT_DEACTIVE = 0x60;
	const unsigned char FONT_MASK_SPACE = 0x00;
	const unsigned char FONT_MASK_CHAR = 0xFF;
	const size_t FONT_TEXTURE_WIDTH = 1024;

    //---------------------------------------------------------------------
	Font::Font(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
		const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader)
		: Ogre::Resource (creator, name, handle, group, isManual, loader),
		mTtfSize(0),
		mTtfResolution(0),
		mSpaceWidth(0),
		mTabWidth(0),
		mCursorWidth(0),
		mDistance(0),
		mAntialiasColour(false),
		mDefaultHeight(0)
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
	Font::GlyphInfo * Font::getGlyphInfo(CodePoint _id)
	{
		for (VectorRangeInfo::iterator iter=mVectorRangeInfo.begin(); iter!=mVectorRangeInfo.end(); ++iter) {
			if ((_id > iter->second) || (_id < iter->first)) continue;
			return & iter->range[_id - iter->first];
		}
		// при ошибках возвращаем пробел
		return & mSpaceGlyphInfo;
		//MYGUI_EXCEPT("Code point " << _id << " not found in font " << mName);
		//return null;
	}
    //---------------------------------------------------------------------
	void Font::loadImpl()
	{
		// create texture
		Ogre::String texName = mName + "_Texture";
		// Create, setting isManual to true and passing self as loader
		mTexture = Ogre::TextureManager::getSingleton().create(texName, mGroup, true, this);
		mTexture->setTextureType(Ogre::TEX_TYPE_2D);
		mTexture->setNumMipmaps(0);
		mTexture->load();

	}
    //---------------------------------------------------------------------
  void Font::unloadImpl()
  {
		// удал€ем все созданные ресурсы
		Ogre::TextureManager::getSingleton().remove(mTexture->getName());
	}
    //---------------------------------------------------------------------
	void Font::loadResource(Ogre::Resource* res)
	{
		// ваще нет ничего
		if (mVectorRangeInfo.empty()) mVectorRangeInfo.push_back(RangeInfo(FONT_CODE_LATIN_START, FONT_CODE_LATIN_END));

		// ManualResourceLoader implementation - load the texture
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

		int max_height = 0/*, max_width = 0*/, max_bear = 0;

		size_t l = 0, m = (mOffsetHeight < 0) ? (-mOffsetHeight) : 0;

		for (VectorRangeInfo::iterator iter=mVectorRangeInfo.begin(); iter!=mVectorRangeInfo.end(); ++iter) {
			for (CodePoint index=iter->first; index<=iter->second; ++index/*, ++glyphCount*/) {

				// символ рисовать ненужно
				if (checkHidePointCode(index)) continue;

				if (FT_Load_Char( face, index, FT_LOAD_RENDER )) continue;
				if (null == face->glyph->bitmap.buffer) continue;
				FT_Int advance = (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 );

				if ( ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY ) > max_height )
					max_height = ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY );

				if ( face->glyph->metrics.horiBearingY > max_bear )
					max_bear = face->glyph->metrics.horiBearingY;

				//if ( (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 ) > max_width)
					//max_width = (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 );

				l += (advance + mDistance);
				if ( (FONT_TEXTURE_WIDTH - 1) < (l + advance) ) { m ++; l = 0;}

			}
		}

		max_height >>= 6;
		max_bear >>= 6;

		//if (max_height & 1) max_height++;
		// пипец без этого пиксель в пиксель никак
		//max_height = mTtfSize;

		// служебные символы
		//glyphCount += 5;
		// ширина служебных равна высоте шрифта
		l += (max_height + mDistance) * 5;

		size_t finalWidth = FONT_TEXTURE_WIDTH;
		size_t finalHeight = (m+1) * (max_height + mDistance);

		// вычисл€ем ближайшую кратную 2
		size_t needHeight = 1;
		while (needHeight < finalHeight) needHeight <<= 1;
		finalHeight = needHeight;

		Ogre::Real textureAspect = (float)finalWidth / (float)finalHeight;

		const size_t pixel_bytes = 2;
		size_t data_width = finalWidth * pixel_bytes;
		size_t data_size = finalWidth * finalHeight * pixel_bytes;

		MYGUI_LOG(Info, "Font '" << mName << "' using texture size " << finalWidth << " x " << finalHeight);
		MYGUI_LOG(Info, "Font '" << mName << "' using real height " << max_height << " pixels");

        Ogre::uchar* imageData = new Ogre::uchar[data_size];
		// Reset content (White, transparent)
        for (size_t i = 0; i < data_size; i += pixel_bytes) {
            imageData[i + 0] = 0xFF; // luminance
            imageData[i + 1] = 0x00; // alpha
        }

		// текущее положение в текстуре
		l = 0;
		m = mOffsetHeight;
		FT_Int advance = 0;

		//------------------------------------------------------------------
		// создаем символ пробела
		//------------------------------------------------------------------
		advance = mSpaceWidth;
		for (int j = 0; j < max_height; j++ ) {
			int row = j + (int)m;
			Ogre::uchar* pDest = &imageData[(row * data_width) + l * pixel_bytes];
			for (int k = 0; k < advance; k++ ) {
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_SPACE;
			}
		}

		mSpaceGlyphInfo.codePoint = FONT_CODE_SPACE;
		mSpaceGlyphInfo.uvRect.left = (Ogre::Real)l / (Ogre::Real)finalWidth;  // u1
		mSpaceGlyphInfo.uvRect.top = (Ogre::Real)m / (Ogre::Real)finalHeight;  // v1
		mSpaceGlyphInfo.uvRect.right = (Ogre::Real)( l + advance ) / (Ogre::Real)finalWidth; // u2
		mSpaceGlyphInfo.uvRect.bottom = ( m + max_height ) / (Ogre::Real)finalHeight; // v2
		mSpaceGlyphInfo.aspectRatio = textureAspect * (mSpaceGlyphInfo.uvRect.right - mSpaceGlyphInfo.uvRect.left)  / (mSpaceGlyphInfo.uvRect.bottom - mSpaceGlyphInfo.uvRect.top);

		l += (advance + mDistance);
		if ( (FONT_TEXTURE_WIDTH - 1) < (l + advance) ) { m += max_height + mDistance;l = 0;}

		//------------------------------------------------------------------
		// создаем табул€цию
		//------------------------------------------------------------------
		advance = mTabWidth;
		for (int j = 0; j < max_height; j++ ) {
			int row = j + (int)m;
			Ogre::uchar* pDest = &imageData[(row * data_width) + l * pixel_bytes];
			for (int k = 0; k < advance; k++ ) {
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_SPACE;
			}
		}

		mTabGlyphInfo.codePoint = FONT_CODE_TAB;
		mTabGlyphInfo.uvRect.left = (Ogre::Real)l / (Ogre::Real)finalWidth;  // u1
		mTabGlyphInfo.uvRect.top = (Ogre::Real)m / (Ogre::Real)finalHeight;  // v1
		mTabGlyphInfo.uvRect.right = (Ogre::Real)( l + advance ) / (Ogre::Real)finalWidth; // u2
		mTabGlyphInfo.uvRect.bottom = ( m + max_height ) / (Ogre::Real)finalHeight; // v2
		mTabGlyphInfo.aspectRatio = textureAspect * (mTabGlyphInfo.uvRect.right - mTabGlyphInfo.uvRect.left)  / (mTabGlyphInfo.uvRect.bottom - mTabGlyphInfo.uvRect.top);

		l += (advance + mDistance);
		if ( (FONT_TEXTURE_WIDTH - 1) < (l + advance) ) { m += max_height + mDistance;l = 0;}

		//------------------------------------------------------------------
		// создаем выделение
		//------------------------------------------------------------------
		advance = mCursorWidth;
		for (int j = 0; j < max_height; j++ ) {
			int row = j + (int)m;
			Ogre::uchar* pDest = &imageData[(row * data_width) + l * pixel_bytes];
			for(int k = 0; k < advance; k++ ) {
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_SELECT;
			}
		}

		mSelectGlyphInfo.codePoint = FONT_CODE_SELECT;
		mSelectGlyphInfo.uvRect.left = (Ogre::Real)l / (Ogre::Real)finalWidth;  // u1
		mSelectGlyphInfo.uvRect.top = (Ogre::Real)m / (Ogre::Real)finalHeight;  // v1
		mSelectGlyphInfo.uvRect.right = (Ogre::Real)( l + advance ) / (Ogre::Real)finalWidth; // u2
		mSelectGlyphInfo.uvRect.bottom = ( m + max_height ) / (Ogre::Real)finalHeight; // v2
		mSelectGlyphInfo.aspectRatio = textureAspect * (mSelectGlyphInfo.uvRect.right - mSelectGlyphInfo.uvRect.left)  / (mSelectGlyphInfo.uvRect.bottom - mSelectGlyphInfo.uvRect.top);

		l += (advance + mDistance);
		if ( (FONT_TEXTURE_WIDTH - 1) < (l + advance) ) { m += max_height + mDistance;l = 0;}

		//------------------------------------------------------------------
		// создаем неактивное выделение
		//------------------------------------------------------------------
		advance = mCursorWidth;
		for (int j = 0; j < max_height; j++ ) {
			int row = j + (int)m;
			Ogre::uchar* pDest = &imageData[(row * data_width) + l * pixel_bytes];
			for(int k = 0; k < advance; k++ ) {
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_SELECT_DEACTIVE;
			}
		}

		mSelectDeactiveGlyphInfo.codePoint = FONT_CODE_SELECT;
		mSelectDeactiveGlyphInfo.uvRect.left = (Ogre::Real)l / (Ogre::Real)finalWidth;  // u1
		mSelectDeactiveGlyphInfo.uvRect.top = (Ogre::Real)m / (Ogre::Real)finalHeight;  // v1
		mSelectDeactiveGlyphInfo.uvRect.right = (Ogre::Real)( l + advance ) / (Ogre::Real)finalWidth; // u2
		mSelectDeactiveGlyphInfo.uvRect.bottom = ( m + max_height ) / (Ogre::Real)finalHeight; // v2
		mSelectDeactiveGlyphInfo.aspectRatio = textureAspect * (mSelectDeactiveGlyphInfo.uvRect.right - mSelectDeactiveGlyphInfo.uvRect.left)  / (mSelectDeactiveGlyphInfo.uvRect.bottom - mSelectDeactiveGlyphInfo.uvRect.top);

		l += (advance + mDistance);
		if ( (FONT_TEXTURE_WIDTH - 1) < (l + advance) ) { m += max_height + mDistance;l = 0;}

		//------------------------------------------------------------------
		// создаем курсор
		//------------------------------------------------------------------
		advance = mCursorWidth;
		for (int j = 0; j < max_height; j++ ) {
			int row = j + (int)m;
			Ogre::uchar* pDest = &imageData[(row * data_width) + l * pixel_bytes];
			for(int k = 0; k < advance; k++ ) {
				*pDest++= FONT_MASK_CHAR;
				*pDest++= FONT_MASK_CHAR;
			}
		}

		mCursorGlyphInfo.codePoint = FONT_CODE_SELECT;
		mCursorGlyphInfo.uvRect.left = (Ogre::Real)l / (Ogre::Real)finalWidth;  // u1
		mCursorGlyphInfo.uvRect.top = (Ogre::Real)m / (Ogre::Real)finalHeight;  // v1
		mCursorGlyphInfo.uvRect.right = (Ogre::Real)( l + advance ) / (Ogre::Real)finalWidth; // u2
		mCursorGlyphInfo.uvRect.bottom = ( m + max_height ) / (Ogre::Real)finalHeight; // v2
		mCursorGlyphInfo.aspectRatio = textureAspect * (mCursorGlyphInfo.uvRect.right - mCursorGlyphInfo.uvRect.left)  / (mCursorGlyphInfo.uvRect.bottom - mCursorGlyphInfo.uvRect.top);

		l += (advance + mDistance);
		if ( (FONT_TEXTURE_WIDTH - 1) < (l + advance) ) { m += max_height + mDistance;l = 0;}

		//------------------------------------------------------------------
		// создаем все остальные символы
		//------------------------------------------------------------------
		FT_Error ftResult;
		for (VectorRangeInfo::iterator iter=mVectorRangeInfo.begin(); iter!=mVectorRangeInfo.end(); ++iter) {

			// устанавливаем размер на весь диапазон
			iter->range.resize(iter->second - iter->first + 1);
			size_t pos = 0;

			for (CodePoint index=iter->first; index<=iter->second; ++index, ++pos) {

				// сомвол рисовать не нада
				if (checkHidePointCode(index)) continue;

				GlyphInfo & info = iter->range.at(pos);

				ftResult = FT_Load_Char( face, index, FT_LOAD_RENDER );
				if (ftResult) {
					// problem loading this glyph, continue
					MYGUI_LOG(Warning, "cannot load character " << index << " in font " << mName);
					continue;
				}

				FT_Int advance = (face->glyph->advance.x >> 6 );// + ( face->glyph->metrics.horiBearingX >> 6 );
				unsigned char* buffer = face->glyph->bitmap.buffer;

				if (null == buffer) {
					// Yuck, FT didn't detect this but generated a null pointer!
					MYGUI_LOG(Warning, "Freetype returned null for character " << index << " in font " << mName);
					continue;
				}

				int y_bearnig = max_bear - ( face->glyph->metrics.horiBearingY >> 6 );

				for(int j = 0; j < face->glyph->bitmap.rows; j++ ) {
					int row = j + (int)m + y_bearnig;
					Ogre::uchar* pDest = &imageData[(row * data_width) + (l + ( face->glyph->metrics.horiBearingX >> 6 )) * pixel_bytes];
					for(int k = 0; k < face->glyph->bitmap.width; k++ ) {
						if (mAntialiasColour) *pDest++= *buffer;
						else *pDest++= FONT_MASK_CHAR;
						*pDest++= *buffer;
						buffer++;
					}
				}

				info.codePoint = index;
				info.uvRect.left = (Ogre::Real)l / (Ogre::Real)finalWidth;  // u1
				info.uvRect.top = (Ogre::Real)(m + mOffsetHeight) / (Ogre::Real)finalHeight;  // v1
				info.uvRect.right = (Ogre::Real)( l + advance ) / (Ogre::Real)finalWidth; // u2
				info.uvRect.bottom = ( m + mOffsetHeight + max_height ) / (Ogre::Real)finalHeight; // v2
				info.aspectRatio = textureAspect * (info.uvRect.right - info.uvRect.left)  / (info.uvRect.bottom - info.uvRect.top);

				l += (advance + mDistance);
				if ( (FONT_TEXTURE_WIDTH - 1) < (l + advance) ) { m += max_height + mDistance;l = 0;}

			}
		}

        Ogre::DataStreamPtr memStream( new Ogre::MemoryDataStream(imageData, data_size, true) );

        Ogre::Image img;
		img.loadRawData( memStream, finalWidth, finalHeight, Ogre::PF_BYTE_LA );

		Ogre::Texture* tex = static_cast<Ogre::Texture*>(res);
		// Call internal _loadImages, not loadImage since that's external and
		// will determine load status etc again, and this is a manual loader inside load()
		Ogre::ConstImagePtrList imagePtrs;
		imagePtrs.push_back(&img);
		tex->_loadImages( imagePtrs );


		FT_Done_FreeType(ftLibrary);
    }
	//-----------------------------------------------------------------------

} // namespace MyGUI
