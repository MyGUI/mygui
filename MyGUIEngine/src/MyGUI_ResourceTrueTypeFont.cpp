/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#include "MyGUI_ResourceTrueTypeFont.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_Bitwise.h"

#ifdef MYGUI_USE_FREETYPE
	#include FT_GLYPH_H
	#include FT_TRUETYPE_TABLES_H
	#include FT_BITMAP_H
#endif // MYGUI_USE_FREETYPE

namespace MyGUI
{

	namespace
	{

		template<typename T>
		void setMax(T& _var, const T& _newValue)
		{
			if (_var < _newValue)
				_var = _newValue;
		}

		std::pair<const Char, const uint8> charMaskData[] =
		{
			std::make_pair(FontCodeType::Selected, (const uint8)'\x88'),
			std::make_pair(FontCodeType::SelectedBack, (const uint8)'\x60'),
			std::make_pair(FontCodeType::Cursor, (const uint8)'\xFF'),
			std::make_pair(FontCodeType::Tab, (const uint8)'\x00')
		};

		const std::map<const Char, const uint8> charMask(charMaskData, charMaskData + sizeof charMaskData / sizeof *charMaskData);

		const uint8 charMaskBlack = (const uint8)'\x00';
		const uint8 charMaskWhite = (const uint8)'\xFF';

		template<bool LAMode>
		struct PixelBase
		{
			// Returns PixelFormat::R8G8B8A8 when LAMode is false, or PixelFormat::L8A8 when LAMode is true.
			static PixelFormat::Enum getFormat();

			// Returns 4 when LAMode is false, or 2 when LAMode is true.
			static size_t getNumBytes();

		protected:
			// Sets a pixel in _dest as 4 or 2 bytes: L8L8L8A8 if LAMode is false, or L8A8 if LAMode is true.
			// Automatically advances _dest just past the pixel written.
			static void set(uint8*& _dest, uint8 _luminance, uint8 _alpha);
		};

		template<>
		struct PixelBase<false>
		{
			static size_t getNumBytes()
			{
				return 4;
			}

			static PixelFormat::Enum getFormat()
			{
				return PixelFormat::R8G8B8A8;
			}

		protected:
			static void set(uint8*& _dest, uint8 _luminance, uint8 _alpha)
			{
				*_dest++ = _luminance;
				*_dest++ = _luminance;
				*_dest++ = _luminance;
				*_dest++ = _alpha;
			}
		};

		template<>
		struct PixelBase<true>
		{
			static size_t getNumBytes()
			{
				return 2;
			}

			static PixelFormat::Enum getFormat()
			{
				return PixelFormat::L8A8;
			}

		protected:
			static void set(uint8*& _dest, uint8 _luminance, uint8 _alpha)
			{
				*_dest++ = _luminance;
				*_dest++ = _alpha;
			}
		};

		template<bool LAMode, bool FromSource = false, bool Antialias = false>
		struct Pixel : PixelBase<LAMode>
		{
			// Sets a pixel in _dest as 4 or 2 bytes: L8L8L8A8 if LAMode is false, or L8A8 if LAMode is true.
			// Sets luminance from _source if both FromSource and Antialias are true; otherwise, uses the specified value.
			// Sets alpha from _source if FromSource is true; otherwise, uses the specified value.
			// Automatically advances _source just past the pixel read if FromSource is true.
			// Automatically advances _dest just past the pixel written.
			static void set(uint8*& _dest, uint8 _luminance, uint8 _alpha, uint8*& _source);
		};

		template<bool LAMode, bool Antialias>
		struct Pixel<LAMode, false, Antialias> : PixelBase<LAMode>
		{
			// Sets the destination pixel using the specified luminance and alpha. Source is ignored, since FromSource is false.
			static void set(uint8*& _dest, uint8 _luminance, uint8 _alpha, uint8* = nullptr)
			{
				PixelBase<LAMode>::set(_dest, _luminance, _alpha);
			}
		};

		template<bool LAMode>
		struct Pixel<LAMode, true, false> : PixelBase<LAMode>
		{
			// Sets the destination pixel using the specified _luminance and using the alpha from the specified source.
			static void set(uint8*& _dest, uint8 _luminance, uint8, uint8*& _source)
			{
				PixelBase<LAMode>::set(_dest, _luminance, *_source++);
			}
		};

		template<bool LAMode>
		struct Pixel<LAMode, true, true> : PixelBase<LAMode>
		{
			// Sets the destination pixel using both the luminance and alpha from the specified source, since Antialias is true.
			static void set(uint8*& _dest, uint8, uint8, uint8*& _source)
			{
				PixelBase<LAMode>::set(_dest, *_source, *_source);
				++_source;
			}
		};

	}

	const int ResourceTrueTypeFont::mGlyphSpacing = 1;
	const float ResourceTrueTypeFont::mSelectedWidth = 1.0f;
	const float ResourceTrueTypeFont::mCursorWidth = 2.0f;

	ResourceTrueTypeFont::ResourceTrueTypeFont() :
		mSize(0),
		mResolution(96),
		mAntialias(false),
		mSpaceWidth(0.0f),
		mTabWidth(0.0f),
		mOffsetHeight(0),
		mDefaultHeight(0),
		mTexture(nullptr)
	{
	}

	ResourceTrueTypeFont::~ResourceTrueTypeFont()
	{
		if (mTexture != nullptr)
		{
			RenderManager::getInstance().destroyTexture(mTexture);
			mTexture = nullptr;
		}
	}

	void ResourceTrueTypeFont::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);

		xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Property")
			{
				const std::string& key = node->findAttribute("key");
				const std::string& value = node->findAttribute("value");
				if (key == "Source") mSource = value;
				else if (key == "Size") mSize = utility::parseFloat(value);
				else if (key == "Resolution") mResolution = utility::parseUInt(value);
				else if (key == "Antialias") mAntialias = utility::parseBool(value);
				else if (key == "SpaceWidth")
				{
					mSpaceWidth = utility::parseFloat(value);
					MYGUI_LOG(Warning, _node->findAttribute("type") << ": Property '" << key << "' in font '" << _node->findAttribute("name") << "' is deprecated; remove it to use automatic calculation.");
				}
				else if (key == "TabWidth") mTabWidth = utility::parseFloat(value);
				else if (key == "OffsetHeight") mOffsetHeight = utility::parseInt(value);
				else if (key == "CursorWidth" || key == "Distance")
				{
					MYGUI_LOG(Warning, _node->findAttribute("type") << ": Property '" << key << "' in font '" << _node->findAttribute("name") << "' is deprecated; value ignored.");
				}
			}
			else if (node->getName() == "Codes")
			{
				// Range of inclusions.
				xml::ElementEnumerator range = node->getElementEnumerator();
				while (range.next("Code"))
				{
					std::string range_value;
					if (range->findAttribute("range", range_value))
					{
						std::vector<std::string> parse_range = utility::split(range_value);
						if (!parse_range.empty())
						{
							Char first = utility::parseUInt(parse_range[0]);
							Char last = parse_range.size() > 1 ? utility::parseUInt(parse_range[1]) : first;
							addCodePointRange(first, last);
						}
					}
				}

				// If no code points have been included, include the Unicode Basic Multilingual Plane by default before processing
				//	any exclusions.
				if (mCharMap.empty())
					addCodePointRange(0, 0xFFFF);

				// Range of exclusions.
				range = node->getElementEnumerator();
				while (range.next("Code"))
				{
					std::string range_value;
					if (range->findAttribute("hide", range_value))
					{
						std::vector<std::string> parse_range = utility::split(range_value);
						if (!parse_range.empty())
						{
							Char first = utility::parseUInt(parse_range[0]);
							Char last = parse_range.size() > 1 ? utility::parseUInt(parse_range[1]) : first;
							removeCodePointRange(first, last);
						}
					}
				}
			}
		}

		initialise();
	}

	GlyphInfo* ResourceTrueTypeFont::getGlyphInfo(Char _id)
	{
#ifdef MYGUI_USE_FREETYPE
		CharMap::iterator charIter = mCharMap.find(_id);
		GlyphMap::iterator glyphIter = mGlyphMap.find((charIter != mCharMap.end()) ? charIter->second : 0);
		return (glyphIter != mGlyphMap.end()) ? &glyphIter->second : nullptr;
#else
		return nullptr;
#endif // MYGUI_USE_FREETYPE
	}

	ITexture* ResourceTrueTypeFont::getTextureFont()
	{
		return mTexture;
	}

	int ResourceTrueTypeFont::getDefaultHeight()
	{
		return mDefaultHeight;
	}

	void ResourceTrueTypeFont::addCodePoint(Char _codePoint)
	{
		mCharMap.insert(CharMap::value_type(_codePoint, 0));
	}

	void ResourceTrueTypeFont::removeCodePoint(Char _codePoint)
	{
		mCharMap.erase(_codePoint);
	}

	void ResourceTrueTypeFont::addCodePointRange(Char _first, Char _second)
	{
		CharMap::iterator positionHint = mCharMap.lower_bound(_first);

		if (positionHint != mCharMap.begin())
			--positionHint;

		for (Char i = _first; i <= _second; ++i)
			positionHint = mCharMap.insert(positionHint, CharMap::value_type(i, 0));
	}

	void ResourceTrueTypeFont::removeCodePointRange(Char _first, Char _second)
	{
		mCharMap.erase(mCharMap.lower_bound(_first), mCharMap.upper_bound(_second));
	}

	void ResourceTrueTypeFont::clearCodePoints()
	{
		mCharMap.clear();
	}

	void ResourceTrueTypeFont::initialise()
	{
#ifndef MYGUI_USE_FREETYPE

		MYGUI_LOG(Error, "ResourceTrueTypeFont: TrueType font '" << getResourceName() << "' disabled. Define MYGUI_USE_FREETYE if you need TrueType fonts.");

#else // MYGUI_USE_FREETYPE

		// If L8A8 (2 bytes per pixel) is supported, use it; otherwise, use R8G8B8A8 (4 bytes per pixel) as L8L8L8A8.
		bool laMode = MyGUI::RenderManager::getInstance().isFormatSupported(Pixel<true>::getFormat(), TextureUsage::Static | TextureUsage::Write);

		// Select and call an appropriate initialisation method. By making this decision up front, we avoid having to branch on
		// these variables many thousands of times inside tight nested loops later. From this point on, the various function
		// templates ensure that all of the necessary branching is done purely at compile time for all combinations.
		int init = (laMode ? 2 : 0) | (mAntialias ? 1 : 0);

		switch (init)
		{
		case 0:
			ResourceTrueTypeFont::initialiseFreeType<false, false>();
			break;
		case 1:
			ResourceTrueTypeFont::initialiseFreeType<false, true>();
			break;
		case 2:
			ResourceTrueTypeFont::initialiseFreeType<true, false>();
			break;
		case 3:
			ResourceTrueTypeFont::initialiseFreeType<true, true>();
			break;
		}

#endif // MYGUI_USE_FREETYPE
	}

#ifdef MYGUI_USE_FREETYPE

	template<bool LAMode, bool Antialias>
	void ResourceTrueTypeFont::initialiseFreeType()
	{
		//-------------------------------------------------------------------//
		// Initialise FreeType and load the font.
		//-------------------------------------------------------------------//

		FT_Library ftLibrary;

		if (FT_Init_FreeType(&ftLibrary) != 0)
			MYGUI_EXCEPT("ResourceTrueTypeFont: Could not init the FreeType library!");

		IDataStream* datastream = DataManager::getInstance().getData(mSource);
		if (datastream == nullptr)
		{
			MYGUI_LOG(Error, "ResourceTrueTypeFont: Could not load the font '" << getResourceName() << "'!");
			return;
		}

		size_t fontBufferSize = datastream->size();
		uint8* fontBuffer = new uint8[fontBufferSize];
		datastream->read(fontBuffer, fontBufferSize);
		delete datastream;

		FT_Face face;

		if (FT_New_Memory_Face(ftLibrary, fontBuffer, (FT_Long)fontBufferSize, 0, &face) != 0)
			MYGUI_EXCEPT("ResourceTrueTypeFont: Could not load the font '" << getResourceName() << "'!");

		// Set the font size by first converting the size to FreeType's 26.6 fixed-point format.
		FT_F26Dot6 ftSize = (FT_F26Dot6)(mSize * (1 << 6));
		if (FT_Set_Char_Size(face, ftSize, 0, mResolution, mResolution) != 0)
			MYGUI_EXCEPT("ResourceTrueTypeFont: Could not set the font size for '" << getResourceName() << "'!");

		//-------------------------------------------------------------------//
		// Calculate the font metrics.
		//-------------------------------------------------------------------//

		// The font's overall ascent and descent are defined in three different places in a TrueType font, and with different
		// values in each place. The most reliable source for these metrics is usually the "usWinAscent" and "usWinDescent" pair of
		// values in the OS/2 header; however, some fonts contain inaccurate data there. To be safe, we use the highest of the two
		// sets of values contained in the OS/2 header and the set of values contained in the horizontal-metrics header ("hhea").
		TT_OS2* os2 = (TT_OS2*)FT_Get_Sfnt_Table(face, ft_sfnt_os2);
		TT_HoriHeader* hhea = (TT_HoriHeader*)FT_Get_Sfnt_Table(face, ft_sfnt_hhea);

		if (os2 == nullptr || hhea == nullptr)
			MYGUI_EXCEPT("ResourceTrueTypeFont: Could not load the font metrics for '" << getResourceName() << "'!");

		int fontAscent = face->size->metrics.ascender >> 6;
		int fontDescent = -face->size->metrics.descender >> 6;

		setMax(fontAscent, (int)((float)os2->usWinAscent * face->size->metrics.y_ppem / face->units_per_EM));
		setMax(fontDescent, (int)((float)os2->usWinDescent * face->size->metrics.y_ppem / face->units_per_EM));

		setMax(fontAscent, (int)((float)os2->sTypoAscender * face->size->metrics.y_ppem / face->units_per_EM));
		setMax(fontDescent, (int)((float)(os2->sTypoDescender + os2->sTypoLineGap) * face->size->metrics.y_ppem / face->units_per_EM));

		// The nominal font height is calculated as the sum of its ascent and descent as specified by the font designer. Previously
		// it was defined by MyGUI in terms of the maximum ascent and descent of the glyphs currently in use, but this caused the
		// font's line spacing to change whenever glyphs were added to or removed from the font definition. Doing it this way
		// instead prevents a lot of layout problems, and it is also more typographically correct and more aesthetically pleasing.
		mDefaultHeight = fontAscent + fontDescent;

		//-------------------------------------------------------------------//
		// Create the glyphs and calculate their metrics.
		//-------------------------------------------------------------------//

		// The maximum glyph height must be at least as much as the default font height, since the special glyphs use the font
		// height as their glyph height. The maximum glyph height will automatically be increased later if taller glyphs are found.
		int maxGlyphHeight = mDefaultHeight;

		GlyphHeightMap glyphHeightMap;
		int texWidth = 0;

		// If no code points have been specified, use the Unicode Basic Multilingual Plane by default.
		if (mCharMap.empty())
			addCodePointRange(0, 0xFFFF);

		// Before creating the glyphs, add a code point that will cause the all-important "Not Defined" glyph to be created. To
		// make sure that our code point doesn't collide with any real code point, we use the highest possible value for a Char.
		addCodePoint(std::numeric_limits<Char>::max());

		// Also add the "Space" code point to force that glyph to be created. For historical reasons, MyGUI users are accustomed to
		// omitting this code point in their font definitions.
		addCodePoint(FontCodeType::Space);

		// Create the standard glyphs.
		for (CharMap::iterator iter = mCharMap.begin(); iter != mCharMap.end(); )
		{
			const Char& codePoint = iter->first;
			FT_UInt glyphIndex = FT_Get_Char_Index(face, codePoint);

			texWidth += createFaceGlyph(glyphIndex, codePoint, fontAscent, face, glyphHeightMap, maxGlyphHeight);

			// If the newly created glyph is the "Not Defined" glyph, remove it from the character map. It doesn't need to be
			// there because getGlyphInfo() already returns the "Not Defined" glyph for any code points that aren't in the map.
			if (iter->second != FontCodeType::NotDefined)
				++iter;
			else
				mCharMap.erase(iter++);
		}

		// Do some special handling for the "Space" and "Tab" glyphs.
		GlyphInfo& spaceGlyphInfo = *getGlyphInfo(FontCodeType::Space);

		if (spaceGlyphInfo.codePoint == FontCodeType::Space)
		{
			// Adjust the width of the "Space" glyph if it has been customized.
			if (mSpaceWidth != 0.0f)
			{
				texWidth += (int)ceil(mSpaceWidth) - (int)ceil(spaceGlyphInfo.width);
				spaceGlyphInfo.width = mSpaceWidth;
				spaceGlyphInfo.advance = mSpaceWidth;
			}

			// If the width of the "Tab" glyph hasn't been customized, make it eight spaces wide.
			if (mTabWidth == 0.0f)
				mTabWidth = 8.0f * spaceGlyphInfo.advance;
		}

		// Create the special glyphs. They must be created last so that they always take precedence in case of a conflict.
		// To make sure that the indices of the special glyphs don't collide with any glyph indices in the TrueType font, we must
		// use glyph indices higher than the highest glyph index in the font.
		FT_UInt nextGlyphIndex = (FT_UInt)face->num_glyphs;

		float height = (float)mDefaultHeight;

		texWidth += createGlyph(nextGlyphIndex++, FontCodeType::Selected, mSelectedWidth, height, 0.0f, 0.0f, 0.0f, glyphHeightMap);
		texWidth += createGlyph(nextGlyphIndex++, FontCodeType::SelectedBack, mSelectedWidth, height, 0.0f, 0.0f, 0.0f, glyphHeightMap);
		texWidth += createGlyph(nextGlyphIndex++, FontCodeType::Cursor, mCursorWidth, height, 0.0f, 0.0f, 0.0f, glyphHeightMap);
		texWidth += createGlyph(nextGlyphIndex++, FontCodeType::Tab, 0.0f, 0.0f, mTabWidth, 0.0f, 0.0f, glyphHeightMap);

		// Calculate the average height of all of the glyphs that are in use. This value will be used for estimating how large the
		// texture needs to be.
		double averageGlyphHeight = 0.0;

		for (GlyphHeightMap::const_iterator j = glyphHeightMap.begin(); j != glyphHeightMap.end(); ++j)
			averageGlyphHeight += j->first * j->second.size();

		averageGlyphHeight /= mGlyphMap.size();

		//-------------------------------------------------------------------//
		// Calculate the final texture size.
		//-------------------------------------------------------------------//

		// Round the current texture width and height up to the nearest powers of two.
		texWidth = Bitwise::firstPO2From(texWidth);
		int texHeight = Bitwise::firstPO2From((int)ceil(averageGlyphHeight) + mGlyphSpacing);

		// At this point, the texture is only one glyph high and is probably very wide. For efficiency reasons, we need to make the
		// texture as square as possible. If the texture cannot be made perfectly square, make it taller than it is wide, because
		// the height may decrease in the final layout due to height packing.
		while (texWidth > texHeight)
		{
			texWidth /= 2;
			texHeight *= 2;
		}

		// Calculate the final layout of all of the glyphs in the texture, packing them tightly by first arranging them by height.
		// We assume that the texture width is fixed but that the texture height can be adjusted up or down depending on how much
		// space is actually needed.
		// In most cases, the final texture will end up square or almost square. In some rare cases, however, we can end up with a
		// texture that's more than twice as high as it is wide; when this happens, we double the width and try again.
		do
		{
			if (texHeight > texWidth * 2)
				texWidth *= 2;

			int texX = 0, texY = 0;

			for (GlyphHeightMap::const_iterator j = glyphHeightMap.begin(); j != glyphHeightMap.end(); ++j)
			{
				for (std::set<std::pair<FT_UInt, GlyphInfo*> >::const_iterator i = j->second.begin(); i != j->second.end(); ++i)
				{
					GlyphInfo& info = *i->second;

					int glyphWidth = (int)ceil(info.width);
					int glyphHeight = (int)ceil(info.height);

					autoWrapGlyphPos(glyphWidth, texWidth, glyphHeight, texX, texY);

					if (glyphWidth > 0)
						texX += mGlyphSpacing + glyphWidth;
				}
			}

			texHeight = Bitwise::firstPO2From(texY + maxGlyphHeight);
		}
		while (texHeight > texWidth * 2);

		//-------------------------------------------------------------------//
		// Create the texture and render the glyphs onto it.
		//-------------------------------------------------------------------//

		mTexture = RenderManager::getInstance().createTexture(MyGUI::utility::toString((size_t)this, "_TrueTypeFont"));

		mTexture->createManual(texWidth, texHeight, TextureUsage::Static | TextureUsage::Write, Pixel<LAMode>::getFormat());

		uint8* texBuffer = static_cast<uint8*>(mTexture->lock(TextureUsage::Write));

		if (texBuffer != nullptr)
		{
			// Make the texture background transparent white.
			for (uint8* dest = texBuffer, * endDest = dest + texWidth * texHeight * Pixel<LAMode>::getNumBytes(); dest != endDest; )
				Pixel<LAMode, false, false>::set(dest, charMaskWhite, charMaskBlack);

			renderGlyphs<LAMode, Antialias>(glyphHeightMap, ftLibrary, face, texBuffer, texWidth, texHeight);

			mTexture->unlock();

			MYGUI_LOG(Info, "ResourceTrueTypeFont: Font '" << getResourceName() << "' using texture size " << texWidth << " x " << texHeight << ".");
			MYGUI_LOG(Info, "ResourceTrueTypeFont: Font '" << getResourceName() << "' using real height " << mDefaultHeight << " pixels.");
		}
		else
		{
			MYGUI_LOG(Error, "ResourceTrueTypeFont: Error locking texture; pointer is nullptr.");
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ftLibrary);
	}

	void ResourceTrueTypeFont::autoWrapGlyphPos(int _glyphWidth, int _texWidth, int _lineHeight, int& _texX, int& _texY)
	{
		if (_glyphWidth > 0 && _texX + mGlyphSpacing + _glyphWidth > _texWidth)
		{
			_texX = 0;
			_texY += mGlyphSpacing + _lineHeight;
		}
	}

	int ResourceTrueTypeFont::createGlyph(FT_UInt _glyphIndex, Char _codePoint, float _width, float _height, float _advance, float _bearingX, float _bearingY, GlyphHeightMap& _glyphHeightMap)
	{
		mCharMap[_codePoint] = _glyphIndex;
		GlyphInfo& info = mGlyphMap.insert(GlyphMap::value_type(_glyphIndex, GlyphInfo(_codePoint, _width, _height, _advance, _bearingX, _bearingY))).first->second;
		_glyphHeightMap[(FT_Pos)_height].insert(std::make_pair(_glyphIndex, &info));

		int width = (int)ceil(_width);

		return (width > 0) ? mGlyphSpacing + width : 0;
	}

	int ResourceTrueTypeFont::createFaceGlyph(FT_UInt _glyphIndex, Char _codePoint, int _fontAscent, const FT_Face& _face, GlyphHeightMap& _glyphHeightMap, int& _maxGlyphHeight)
	{
		if (mGlyphMap.find(_glyphIndex) == mGlyphMap.end() && FT_Load_Glyph(_face, _glyphIndex, FT_LOAD_DEFAULT) == 0)
		{
			float width = _face->glyph->metrics.width / 64.0f;
			float height = _face->glyph->metrics.height / 64.0f;

			float bearingX = _face->glyph->metrics.horiBearingX / 64.0f;
			float bearingY = _fontAscent - (_face->glyph->metrics.horiBearingY / 64.0f) - mOffsetHeight;

			float advance = (_face->glyph->advance.x / 64.0f) - bearingX;

			// The following calculations aren't currently needed but are kept here for future use.
			// float ascent = _face->glyph->metrics.horiBearingY / 64.0f;
			// float descent = height - ascent;

			setMax(_maxGlyphHeight, (int)height);

			return createGlyph(_glyphIndex, _codePoint, width, height, advance, bearingX, bearingY, _glyphHeightMap);
		}

		return 0;
	}

	template<bool LAMode, bool Antialias>
	void ResourceTrueTypeFont::renderGlyphs(const GlyphHeightMap& _glyphHeightMap, const FT_Library& _ftLibrary, const FT_Face& _face, uint8* _texBuffer, int _texWidth, int _texHeight)
	{
		FT_Bitmap ftBitmap;
		FT_Bitmap_New(&ftBitmap);

		int texX = 0, texY = 0;

		for (GlyphHeightMap::const_iterator j = _glyphHeightMap.begin(); j != _glyphHeightMap.end(); ++j)
		{
			for (std::set<std::pair<FT_UInt, GlyphInfo*> >::const_iterator i = j->second.begin(); i != j->second.end(); ++i)
			{
				GlyphInfo& info = *i->second;

				switch (info.codePoint)
				{
				case FontCodeType::Selected:
				case FontCodeType::SelectedBack:
				{
					renderGlyph<LAMode, false, false>(info, charMaskWhite, charMaskBlack, charMask.at(info.codePoint), j->first, _texBuffer, _texWidth, _texHeight, texX, texY);
					FloatRect& uvRect = getGlyphInfo(info.codePoint)->uvRect;
					uvRect.right = uvRect.left;
					uvRect.top += (uvRect.bottom - uvRect.top) / 2.0f;
					uvRect.bottom = uvRect.top;
				}
				break;

				case FontCodeType::Cursor:
				{
					renderGlyph<LAMode, false, false>(info, charMaskWhite, charMaskBlack, charMask.at(info.codePoint), j->first, _texBuffer, _texWidth, _texHeight, texX, texY);
					FloatRect& uvRect = getGlyphInfo(info.codePoint)->uvRect;
					uvRect.top += (uvRect.bottom - uvRect.top) / 2.0f;
					uvRect.bottom = uvRect.top;
				}
				break;

				case FontCodeType::Tab:
					renderGlyph<LAMode, false, false>(info, charMaskWhite, charMaskBlack, charMask.at(info.codePoint), j->first, _texBuffer, _texWidth, _texHeight, texX, texY);
					break;

				default:
					if (FT_Load_Glyph(_face, i->first, FT_LOAD_RENDER) == 0)
					{
						if (_face->glyph->bitmap.buffer != nullptr)
						{
							uint8* glyphBuffer = nullptr;

							switch (_face->glyph->bitmap.pixel_mode)
							{
							case FT_PIXEL_MODE_GRAY:
								glyphBuffer = _face->glyph->bitmap.buffer;
								break;

							case FT_PIXEL_MODE_MONO:
								// Convert the monochrome bitmap to 8-bit before rendering it.
								if (FT_Bitmap_Convert(_ftLibrary, &_face->glyph->bitmap, &ftBitmap, 1) == 0)
								{
									// Go through the bitmap and convert all of the nonzero values to 0xFF (white).
									for (uint8* p = ftBitmap.buffer, * endP = p + ftBitmap.width * ftBitmap.rows; p != endP; ++p)
										*p ^= -*p ^ *p;

									glyphBuffer = ftBitmap.buffer;
								}
								break;
							}

							if (glyphBuffer != nullptr)
								renderGlyph<LAMode, true, Antialias>(info, charMaskWhite, charMaskWhite, charMaskWhite, j->first, _texBuffer, _texWidth, _texHeight, texX, texY, glyphBuffer);
						}
					}
					else
					{
						MYGUI_LOG(Warning, "ResourceTrueTypeFont: Cannot load glyph " << i->first << " for character " << info.codePoint << " in font '" << getResourceName() << "'.");
					}
					break;
				}
			}
		}

		FT_Bitmap_Done(_ftLibrary, &ftBitmap);
	}

	template<bool LAMode, bool UseBuffer, bool Antialias>
	void ResourceTrueTypeFont::renderGlyph(GlyphInfo& _info, uint8 _luminance0, uint8 _luminance1, uint8 _alpha, int _lineHeight, uint8* _texBuffer, int _texWidth, int _texHeight, int& _texX, int& _texY, uint8* _glyphBuffer)
	{
		int width = (int)ceil(_info.width);
		int height = (int)ceil(_info.height);

		autoWrapGlyphPos(width, _texWidth, _lineHeight, _texX, _texY);

		uint8* dest = _texBuffer + (_texY * _texWidth + _texX) * Pixel<LAMode>::getNumBytes();

		// Calculate how much to advance the destination pointer after each row to get to the start of the next row.
		ptrdiff_t destNextRow = (_texWidth - width) * Pixel<LAMode>::getNumBytes();

		for (int j = height; j > 0; --j)
		{
			int i;
			for (i = width; i > 1; i -= 2)
			{
				Pixel<LAMode, UseBuffer, Antialias>::set(dest, _luminance0, _alpha, _glyphBuffer);
				Pixel<LAMode, UseBuffer, Antialias>::set(dest, _luminance1, _alpha, _glyphBuffer);
			}

			if (i > 0)
				Pixel<LAMode, UseBuffer, Antialias>::set(dest, _luminance0, _alpha, _glyphBuffer);

			dest += destNextRow;
		}

		// Calculate and store the glyph's UV coordinates within the texture.
		_info.uvRect.left = (float) _texX / _texWidth; // u1
		_info.uvRect.top = (float) _texY / _texHeight; // v1
		_info.uvRect.right = (float)(_texX + _info.width) / _texWidth; // u2
		_info.uvRect.bottom = (float)(_texY + _info.height) / _texHeight; // v2

		if (width > 0)
			_texX += mGlyphSpacing + width;
	}

#endif // MYGUI_USE_FREETYPE

} // namespace MyGUI
