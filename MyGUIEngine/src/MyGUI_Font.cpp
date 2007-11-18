
#include "OgreStableHeaders.h"

#include "MyGUI_Font.h"
#include "Common.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTexture.h"
#include "OgreResourceGroupManager.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreRenderWindow.h"
#include "OgreException.h"
#include "OgreBlendMode.h"
#include "OgreTextureUnitState.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreMaterial.h"
#include "OgreBitwise.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H



namespace MyGUI
{

    //---------------------------------------------------------------------
	Font::Font(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
		const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader)
		: Ogre::Resource (creator, name, handle, group, isManual, loader),
		mTtfSize(0), mTtfResolution(0), mAntialiasColour(false),
		mSpaceSimbol('a'), mCountSpaceTab(4)
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
    void Font::setSource(const String& source)
    {
        mSource = source;
    }
    //---------------------------------------------------------------------
    void Font::setTrueTypeSize(Ogre::Real ttfSize)
    {
        mTtfSize = ttfSize;
    }
    //---------------------------------------------------------------------
    void Font::setTrueTypeResolution(Ogre::uint ttfResolution)
    {
        mTtfResolution = ttfResolution;
    }
    //---------------------------------------------------------------------
    const String& Font::getSource(void) const
    {
        return mSource;
    }
    //---------------------------------------------------------------------
    Ogre::Real Font::getTrueTypeSize(void) const
    {
        return mTtfSize;
    }
    //---------------------------------------------------------------------
    Ogre::uint Font::getTrueTypeResolution(void) const
    {
        return mTtfResolution;
    }
	//---------------------------------------------------------------------
	const Font::GlyphInfo& Font::getGlyphInfo(CodePoint id) const
	{
		CodePointMap::const_iterator i = mCodePointMap.find(id);
		if (i == mCodePointMap.end()) {
			OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, 
				"Code point " + Ogre::StringConverter::toString(id) + " not found in font " + mName, "Font::getGlyphInfo");
		}
		return i->second;
	}
    //---------------------------------------------------------------------
    void Font::loadImpl()
    {
		// create texture
		String texName = mName + "Texture";
		// Create, setting isManual to true and passing self as loader
		mTexture = Ogre::TextureManager::getSingleton().create(texName, mGroup, true, this);
		mTexture->setTextureType(Ogre::TEX_TYPE_2D);
		mTexture->setNumMipmaps(0);
		mTexture->load();

		// create new material for simple text
		mpMaterial =  Ogre::MaterialManager::getSingleton().create("Fonts/" + mName,  mGroup);
		if (mpMaterial.isNull())
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error creating new material!", "Font::load" );

		Ogre::TextureUnitState* texLayer = mpMaterial->getTechnique(0)->getPass(0)->createTextureUnitState( texName );
		// Clamp to avoid fuzzy edges
        texLayer->setTextureAddressingMode( Ogre::TextureUnitState::TAM_CLAMP );
		// Allow min/mag filter, but no mip
		texLayer->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
        mpMaterial->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );

		// create new material for edit text
		mpMaterialSelectedFont =  Ogre::MaterialManager::getSingleton().create("FontsSelected/" + mName,  mGroup);
		if (mpMaterialSelectedFont.isNull()) 
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error creating new material!", "Font::load" );

		texLayer = mpMaterialSelectedFont->getTechnique(0)->getPass(0)->createTextureUnitState( texName, 0 );
		// Clamp to avoid fuzzy edges
        texLayer->setTextureAddressingMode( Ogre::TextureUnitState::TAM_CLAMP );
		// Allow min/mag filter, but no mip
		texLayer->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
        mpMaterialSelectedFont->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );

		texLayer = mpMaterialSelectedFont->getTechnique(0)->getPass(0)->createTextureUnitState( texName, 1 );
		// Clamp to avoid fuzzy edges
        texLayer->setTextureAddressingMode( Ogre::TextureUnitState::TAM_CLAMP );
		// Allow min/mag filter, but no mip
		texLayer->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);

		texLayer->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_CURRENT, Ogre::LBS_CURRENT);
		texLayer->setAlphaOperation(Ogre::LBX_BLEND_TEXTURE_ALPHA, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);

	}
    //---------------------------------------------------------------------
    void Font::unloadImpl()
    {
		// Cascade to the texture we created
        mTexture->unload();
    }
    //---------------------------------------------------------------------
	void Font::loadResource(Ogre::Resource* res)
	{
		// добавл€ем пробел
		addCodePointRange(CodePointRange(FONT_CODE_SPACE, FONT_CODE_SPACE));
		// добавл€ем знак выделени€
		addCodePointRange(CodePointRange(FONT_CODE_SELECT, FONT_CODE_SELECT));
		// добавл€ем знак табул€ции
		addCodePointRange(CodePointRange(FONT_CODE_TAB, FONT_CODE_TAB));

		// ManualResourceLoader implementation - load the texture
		FT_Library ftLibrary;
		// Init freetype
        if( FT_Init_FreeType( &ftLibrary ) )
            OGRE_EXCEPT( Ogre::Exception::ERR_INTERNAL_ERROR, "Could not init FreeType library!", "Font::Font");

        FT_Face face;
        // Add a gap between letters vert and horz
        // prevents nasty artefacts when letters are too close together
        Ogre::uint char_spacer = 5;

        // Locate ttf file, load it pre-buffered into memory by wrapping the
		// original DataStream in a MemoryDataStream
		Ogre::DataStreamPtr dataStreamPtr =
			Ogre::ResourceGroupManager::getSingleton().openResource(
				mSource, mGroup, true, this);
		Ogre::MemoryDataStream ttfchunk(dataStreamPtr);

        // Load font
        if( FT_New_Memory_Face( ftLibrary, ttfchunk.getPtr(), (FT_Long)ttfchunk.size() , 0, &face ) )
            OGRE_EXCEPT( Ogre::Exception::ERR_INTERNAL_ERROR,
            "Could not open font face!", "Font::createTextureFromFont" );


        // Convert our point size to freetype 26.6 fixed point format
        FT_F26Dot6 ftSize = (FT_F26Dot6)(mTtfSize * (1 << 6));
        if( FT_Set_Char_Size( face, ftSize, 0, mTtfResolution, mTtfResolution ) )
            OGRE_EXCEPT( Ogre::Exception::ERR_INTERNAL_ERROR,
            "Could not set char size!", "Font::createTextureFromFont" );

        int max_height = 0, max_width = 0, max_bear = 0;

		// Backwards compatibility - if codepoints not supplied, assume 33-166
		if (mCodePointRangeList.empty()) mCodePointRangeList.push_back(CodePointRange(33, 166));

		// Calculate maximum width, height and bearing
		size_t glyphCount = 0;
		for (CodePointRangeList::const_iterator r = mCodePointRangeList.begin(); r != mCodePointRangeList.end(); ++r) {

			const CodePointRange& range = *r;
			for(CodePoint cp = range.first; cp <= range.second; ++cp, ++glyphCount) {
				FT_Load_Char( face, cp, FT_LOAD_RENDER );

				if ( ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY ) > max_height )
					max_height = ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY );

				if ( face->glyph->metrics.horiBearingY > max_bear )
					max_bear = face->glyph->metrics.horiBearingY;

				if ( (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 ) > max_width)
					max_width = (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 );
			}

		}

		// Now work out how big our texture needs to be
		size_t rawSize = (max_width + char_spacer) * ((max_height >> 6) + char_spacer) * glyphCount;

		uint32 tex_side = static_cast<uint32>(Ogre::Math::Sqrt(rawSize));
		// just in case the size might chop a glyph in half, add another glyph width/height
		tex_side += std::max(max_width, (max_height>>6));
		// Now round up to nearest power of two
		Ogre::uint32 roundUpSize = Ogre::Bitwise::firstPO2From(tex_side);

		// Would we benefit from using a non-square texture (2X width(
		size_t finalWidth, finalHeight;

		if (roundUpSize*roundUpSize*0.5 >= rawSize) finalHeight = roundUpSize * 0.5;
		else finalHeight = roundUpSize;

		finalWidth = roundUpSize;

		Ogre::Real textureAspect = finalWidth / finalHeight;

		const size_t pixel_bytes = 2;
		size_t data_width = finalWidth * pixel_bytes;
		size_t data_size = finalWidth * finalHeight * pixel_bytes;

		LOG("Font '", mName, "' using texture size ", util::toString(finalWidth), " x ", util::toString(finalHeight));

        Ogre::uchar* imageData = new Ogre::uchar[data_size];
		// Reset content (White, transparent)
        for (size_t i = 0; i < data_size; i += pixel_bytes) {
            imageData[i + 0] = 0xFF; // luminance
            imageData[i + 1] = 0x00; // alpha
        }

        size_t l = 0, m = 0;
		for (CodePointRangeList::const_iterator r = mCodePointRangeList.begin(); r != mCodePointRangeList.end(); ++r) {

			const CodePointRange& range = *r;
			for(CodePoint cp = range.first; cp <= range.second; ++cp ) {
				FT_Error ftResult;

				// сохран€ем оригинильный код
				CodePoint original = cp;

				// символ дл€ выделени€
				bool selectedChar = (FONT_CODE_SELECT == cp);
				bool spaceChar = (FONT_CODE_SPACE == cp);
				bool tabChar = (FONT_CODE_TAB == cp);

				// подмен€ем символ дл€ расчетов
				if (selectedChar || spaceChar || tabChar) cp = mSpaceSimbol;

				// Load & render glyph
				ftResult = FT_Load_Char( face, cp, FT_LOAD_RENDER );
				if (ftResult) {
					// problem loading this glyph, continue
					LOG("Info: cannot load character ", util::toString(cp), " in font ", mName);
					continue;
				}

				FT_Int advance = (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 );

				unsigned char* buffer = face->glyph->bitmap.buffer;

				if (null == buffer) {
					// Yuck, FT didn't detect this but generated a null pointer!
					LOG("Info: Freetype returned null for character ", util::toString(cp), " in font ", mName);
					continue;
				}

				int y_bearnig = ( max_bear >> 6 ) - ( face->glyph->metrics.horiBearingY >> 6 );

				for(int j = 0; j < face->glyph->bitmap.rows; j++ ) {

					int row = j + (int)m + y_bearnig;
					Ogre::uchar* pDest = &imageData[(row * data_width) + l * pixel_bytes];
					for(int k = 0; k < face->glyph->bitmap.width; k++ ) {

						if (spaceChar) {
							*pDest++= 0xFF;
							*pDest++= 0x00;
						} else if (selectedChar) {
							*pDest++= 0xFF;
							*pDest++= 0x88;
						} else {
							if (mAntialiasColour) *pDest++= *buffer;
							else *pDest++= 0xFF;
							*pDest++= *buffer; 
						}
						buffer++;

					}
				}

				this->setGlyphTexCoords(original,
					(Ogre::Real)l / (Ogre::Real)finalWidth,  // u1
					(Ogre::Real)m / (Ogre::Real)finalHeight,  // v1
					(Ogre::Real)( l + ( face->glyph->advance.x >> 6 ) ) / (Ogre::Real)finalWidth, // u2
					( m + ( max_height >> 6 ) ) / (Ogre::Real)finalHeight, // v2
					(tabChar ? textureAspect * (float)mCountSpaceTab : textureAspect)
					);

				// Advance a column
				l += (advance + char_spacer);

				// If at end of row
				if( finalWidth - 1 < l + ( advance ) ) {
					m += ( max_height >> 6 ) + char_spacer;
					l = 0;
				}
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

}
