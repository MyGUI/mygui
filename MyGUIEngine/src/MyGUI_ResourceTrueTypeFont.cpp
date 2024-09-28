/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourceTrueTypeFont.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_DataStreamHolder.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_Bitwise.h"

#ifdef MYGUI_USE_FREETYPE

	#include FT_GLYPH_H
	#include FT_TRUETYPE_TABLES_H
	#include FT_BITMAP_H
	#include FT_WINFONTS_H

	#ifdef MYGUI_MSDF_FONTS
		#include "msdfgen/msdfgen.h"
		#include "msdfgen/msdfgen-ext.h"
	#endif

#endif // MYGUI_USE_FREETYPE

namespace MyGUI
{

#ifndef MYGUI_USE_FREETYPE
	ResourceTrueTypeFont::~ResourceTrueTypeFont() = default;

	void ResourceTrueTypeFont::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);
		MYGUI_LOG(
			Error,
			"ResourceTrueTypeFont: TrueType font '"
				<< getResourceName() << "' disabled. Define MYGUI_USE_FREETYE if you need TrueType fonts.");
	}

	const GlyphInfo* ResourceTrueTypeFont::getGlyphInfo(Char _id) const
	{
		return nullptr;
	}

	ITexture* ResourceTrueTypeFont::getTextureFont() const
	{
		return nullptr;
	}

	int ResourceTrueTypeFont::getDefaultHeight() const
	{
		return 0;
	}

	void ResourceTrueTypeFont::textureInvalidate(ITexture* _texture)
	{
	}

	std::vector<std::pair<Char, Char>> ResourceTrueTypeFont::getCodePointRanges() const
	{
		return std::vector<std::pair<Char, Char>>();
	}

	Char ResourceTrueTypeFont::getSubstituteCodePoint() const
	{
		return Char();
	}

	void ResourceTrueTypeFont::initialise()
	{
	}

	void ResourceTrueTypeFont::setSource(std::string_view _value)
	{
	}

	void ResourceTrueTypeFont::setShader(std::string_view _value)
	{
	}

	void ResourceTrueTypeFont::setSize(float _value)
	{
	}

	void ResourceTrueTypeFont::setResolution(unsigned int _value)
	{
	}

	void ResourceTrueTypeFont::setHinting(std::string_view _value)
	{
	}

	void ResourceTrueTypeFont::setAntialias(bool _value)
	{
	}

	void ResourceTrueTypeFont::setTabWidth(float _value)
	{
	}

	void ResourceTrueTypeFont::setOffsetHeight(int _value)
	{
	}

	void ResourceTrueTypeFont::setSubstituteCode(int _value)
	{
	}

	void ResourceTrueTypeFont::setDistance(int _value)
	{
	}

	void ResourceTrueTypeFont::setMsdfMode(bool _value)
	{
	}

	void ResourceTrueTypeFont::setMsdfRange(int _value)
	{
	}

	void ResourceTrueTypeFont::addCodePointRange(Char _first, Char _second)
	{
	}

	void ResourceTrueTypeFont::removeCodePointRange(Char _first, Char _second)
	{
	}

#else // MYGUI_USE_FREETYPE
	namespace
	{

		template<typename T>
		void setMax(T& _var, const T& _newValue)
		{
			if (_var < _newValue)
				_var = _newValue;
		}

		const std::map<const Char, const uint8> charMask{
			{FontCodeType::Selected, 0x88},
			{FontCodeType::SelectedBack, 0x60},
			{FontCodeType::Cursor, 0xFF},
			{FontCodeType::Tab, 0x00}};
		const uint8 charMaskBlack = 0x00;
		const uint8 charMaskWhite = 0xFF;

		template<bool LAMode>
		struct PixelBase
		{
			static PixelFormat::Enum getFormat()
			{
				if constexpr (LAMode)
					return PixelFormat::L8A8;
				else
					return PixelFormat::R8G8B8A8;
			}

			static size_t getNumBytes()
			{
				if constexpr (LAMode)
					return 2;
				else
					return 4;
			}

		protected:
			// Sets a pixel in _dest as 4 or 2 bytes: L8L8L8A8 if LAMode is false, or L8A8 if LAMode is true.
			// Automatically advances _dest just past the pixel written.
			static void set(uint8*& _dest, uint8 _luminance, uint8 _alpha)
			{
				if constexpr (LAMode)
				{
					*_dest++ = _luminance;
					*_dest++ = _alpha;
				}
				else
				{
					*_dest++ = _luminance;
					*_dest++ = _luminance;
					*_dest++ = _luminance;
					*_dest++ = _alpha;
				}
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
			static void set(uint8*& _dest, uint8 _luminance, uint8 _alpha, uint8*& _source)
			{
				if constexpr (FromSource)
				{
					(void)_alpha;
					if constexpr (Antialias)
					{
						(void)_luminance;
						// Sets the destination pixel using both the luminance and alpha from the specified source.
						PixelBase<LAMode>::set(_dest, *_source, *_source);
					}
					else
					{
						// Sets the destination pixel using the specified _luminance and alpha from the specified source.
						PixelBase<LAMode>::set(_dest, _luminance, *_source);
					}
					++_source;
				}
				else
				{
					(void)_source;
					// Sets the destination pixel using the specified luminance and alpha. Source is ignored.
					PixelBase<LAMode>::set(_dest, _luminance, _alpha);
				}
			}
		};
	}

	const int ResourceTrueTypeFont::mDefaultGlyphSpacing = 1;
	const float ResourceTrueTypeFont::mDefaultTabWidth = 8.0f;
	const float ResourceTrueTypeFont::mSelectedWidth = 1.0f;
	const float ResourceTrueTypeFont::mCursorWidth = 2.0f;

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
				std::string_view key = node->findAttribute("key");
				std::string_view value = node->findAttribute("value");
				if (key == "Source")
					setSource(value);
				else if (key == "Shader")
					setShader(value);
				else if (key == "Size")
					setSize(utility::parseFloat(value));
				else if (key == "Resolution")
					setResolution(utility::parseUInt(value));
				else if (key == "Antialias")
					setAntialias(utility::parseBool(value));
				else if (key == "TabWidth")
					setTabWidth(utility::parseFloat(value));
				else if (key == "OffsetHeight")
					setOffsetHeight(utility::parseInt(value));
				else if (key == "SubstituteCode")
					setSubstituteCode(utility::parseInt(value));
				else if (key == "Distance")
					setDistance(utility::parseInt(value));
				else if (key == "Hinting")
					setHinting(value);
				else if (key == "SpaceWidth")
				{
					mSpaceWidth = utility::parseFloat(value);
					MYGUI_LOG(
						Warning,
						_node->findAttribute("type")
							<< ": Property '" << key << "' in font '" << _node->findAttribute("name")
							<< "' is deprecated; remove it to use automatic calculation.");
				}
				else if (key == "CursorWidth")
				{
					MYGUI_LOG(
						Warning,
						_node->findAttribute("type")
							<< ": Property '" << key << "' in font '" << _node->findAttribute("name")
							<< "' is deprecated; value ignored.");
				}
				else if (key == "MsdfMode")
				{
					setMsdfMode(utility::parseBool(value));
				}
				else if (key == "MsdfRange")
				{
					setMsdfRange(utility::parseInt(value));
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

	const GlyphInfo* ResourceTrueTypeFont::getGlyphInfo(Char _id) const
	{
		GlyphMap::const_iterator glyphIter = mGlyphMap.find(_id);

		if (glyphIter != mGlyphMap.end())
		{
			return &glyphIter->second;
		}

		return mSubstituteGlyphInfo;
	}

	ITexture* ResourceTrueTypeFont::getTextureFont() const
	{
		return mTexture;
	}

	int ResourceTrueTypeFont::getDefaultHeight() const
	{
		return mDefaultHeight;
	}

	void ResourceTrueTypeFont::textureInvalidate(ITexture* _texture)
	{
		mGlyphMap.clear();
		initialise();
	}

	std::vector<std::pair<Char, Char>> ResourceTrueTypeFont::getCodePointRanges() const
	{
		std::vector<std::pair<Char, Char>> result;

		if (!mCharMap.empty())
		{
			CharMap::const_iterator iter = mCharMap.begin();
			CharMap::const_iterator endIter = mCharMap.end();

			// Start the first range with the first code point in the map.
			Char rangeBegin = iter->first;
			Char rangeEnd = rangeBegin;

			// Loop over the rest of the map and find the contiguous ranges.
			for (++iter; iter != endIter; ++iter)
			{
				if (iter->first == rangeEnd + 1)
				{
					// Extend the current range.
					++rangeEnd;
				}
				else
				{
					// Found the start of a new range. First, save the current range.
					result.emplace_back(rangeBegin, rangeEnd);

					// Then start the new range.
					rangeBegin = rangeEnd = iter->first;
				}
			}

			// Save the final range.
			result.emplace_back(rangeBegin, rangeEnd);
		}

		return result;
	}

	Char ResourceTrueTypeFont::getSubstituteCodePoint() const
	{
		return mSubstituteCodePoint;
	}

	void ResourceTrueTypeFont::addCodePoint(Char _codePoint)
	{
		mCharMap.insert(CharMap::value_type(_codePoint, 0));
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

	void ResourceTrueTypeFont::initialise()
	{
		if (mGlyphSpacing == -1)
			mGlyphSpacing = mDefaultGlyphSpacing;

		// If L8A8 (2 bytes per pixel) is supported, use it; otherwise, use R8G8B8A8 (4 bytes per pixel) as L8L8L8A8.
		bool laMode = MyGUI::RenderManager::getInstance().isFormatSupported(
			Pixel<true>::getFormat(),
			TextureUsage::Static | TextureUsage::Write);
		if (mMsdfMode)
			laMode = false;

		// Select and call an appropriate initialisation method. By making this decision up front, we avoid having to branch on
		// these variables many thousands of times inside tight nested loops later. From this point on, the various function
		// templates ensure that all the necessary branching is done purely at compile time for all combinations.
		if (laMode)
		{
			if (mAntialias)
				ResourceTrueTypeFont::initialiseFreeType<true, true>();
			else
				ResourceTrueTypeFont::initialiseFreeType<true, false>();
		}
		else
		{
			if (mAntialias)
				ResourceTrueTypeFont::initialiseFreeType<false, true>();
			else
				ResourceTrueTypeFont::initialiseFreeType<false, false>();
		}
	}

	template<bool LAMode, bool Antialias>
	void ResourceTrueTypeFont::initialiseFreeType()
	{
		//-------------------------------------------------------------------//
		// Initialise FreeType and load the font.
		//-------------------------------------------------------------------//

		FT_Library ftLibrary;

		if (FT_Init_FreeType(&ftLibrary) != 0)
			MYGUI_EXCEPT("ResourceTrueTypeFont: Could not init the FreeType library!");

		uint8* fontBuffer = nullptr;

		FT_Face ftFace = loadFace(ftLibrary, fontBuffer);

		if (ftFace == nullptr)
		{
			MYGUI_LOG(Error, "ResourceTrueTypeFont: Could not load the font '" << getResourceName() << "'!");
			FT_Done_FreeType(ftLibrary);
			return;
		}

	#ifdef MYGUI_MSDF_FONTS
		msdfgen::FontHandle* msdfFont = nullptr;

		if (mMsdfMode)
		{
			msdfFont = msdfgen::adoptFreetypeFont(ftFace);
		}
	#endif

		//-------------------------------------------------------------------//
		// Calculate the font metrics.
		//-------------------------------------------------------------------//

		// The font's overall ascent and descent are defined in three different places in a TrueType font, and with different
		// values in each place. The most reliable source for these metrics is usually the "usWinAscent" and "usWinDescent" pair of
		// values in the OS/2 header; however, some fonts contain inaccurate data there. To be safe, we use the highest of the set
		// of values contained in the face metrics and the two sets of values contained in the OS/2 header.
		int fontAscent = ftFace->size->metrics.ascender >> 6;
		int fontDescent = -ftFace->size->metrics.descender >> 6;

		TT_OS2* os2 = (TT_OS2*)FT_Get_Sfnt_Table(ftFace, ft_sfnt_os2);

		if (os2 != nullptr)
		{
			setMax(fontAscent, os2->usWinAscent * ftFace->size->metrics.y_ppem / ftFace->units_per_EM);
			setMax(fontDescent, os2->usWinDescent * ftFace->size->metrics.y_ppem / ftFace->units_per_EM);

			setMax(fontAscent, os2->sTypoAscender * ftFace->size->metrics.y_ppem / ftFace->units_per_EM);
			setMax(fontDescent, -os2->sTypoDescender * ftFace->size->metrics.y_ppem / ftFace->units_per_EM);
		}

		// The nominal font height is calculated as the sum of its ascent and descent as specified by the font designer. Previously
		// it was defined by MyGUI in terms of the maximum ascent and descent of the glyphs currently in use, but this caused the
		// font's line spacing to change whenever glyphs were added to or removed from the font definition. Doing it this way
		// instead prevents a lot of layout problems, and it is also more typographically correct and more aesthetically pleasing.
		mDefaultHeight = fontAscent + fontDescent;

		// Set the load flags based on the specified type of hinting.
		FT_Int32 ftLoadFlags = FT_LOAD_DEFAULT;

		switch (mHinting)
		{
		case HintingForceAuto: ftLoadFlags = FT_LOAD_FORCE_AUTOHINT; break;
		case HintingDisableAuto: ftLoadFlags = FT_LOAD_NO_AUTOHINT; break;
		case HintingDisableAll:
			// When hinting is completely disabled, glyphs must always be rendered -- even during layout calculations -- due to
			// discrepancies between the glyph metrics and the actual rendered bitmap metrics.
			ftLoadFlags = FT_LOAD_NO_HINTING | FT_LOAD_RENDER;
			break;
		case HintingUseNative: ftLoadFlags = FT_LOAD_DEFAULT; break;
		}

		//-------------------------------------------------------------------//
		// Create the glyphs and calculate their metrics.
		//-------------------------------------------------------------------//

		GlyphHeightMap glyphHeightMap;
		int texWidth = 0;

		// Before creating the glyphs, add the "Space" code point to force that glyph to be created. For historical reasons, MyGUI
		// users are accustomed to omitting this code point in their font definitions.
		addCodePoint(FontCodeType::Space);

		// Create the standard glyphs.
		for (CharMap::iterator iter = mCharMap.begin(); iter != mCharMap.end();)
		{
			const Char& codePoint = iter->first;
			FT_UInt glyphIndex = FT_Get_Char_Index(ftFace, codePoint);

			if (!mMsdfMode)
				texWidth += createFaceGlyph(glyphIndex, codePoint, fontAscent, ftFace, ftLoadFlags, glyphHeightMap);
	#ifdef MYGUI_MSDF_FONTS
			else
				texWidth += createMsdfFaceGlyph(codePoint, fontAscent, msdfFont, glyphHeightMap);
	#endif

			// If the newly created glyph is the "Not Defined" glyph, it means that the code point is not supported by the font.
			// Remove it from the character map so that we can provide our own substitute instead of letting FreeType do it.
			if (iter->second != 0)
				++iter;
			else
				mCharMap.erase(iter++);
		}

		// Do some special handling for the "Space" and "Tab" glyphs.
		GlyphMap::iterator spaceGlyphIter = mGlyphMap.find(FontCodeType::Space);

		if (spaceGlyphIter != mGlyphMap.end())
		{
			// Adjust the width of the "Space" glyph if it has been customized.
			if (mSpaceWidth != 0.0f)
			{
				texWidth += (int)std::ceil(mSpaceWidth) - (int)std::ceil(spaceGlyphIter->second.width);
				spaceGlyphIter->second.width = mSpaceWidth;
				spaceGlyphIter->second.advance = mSpaceWidth;
			}

			// If the width of the "Tab" glyph hasn't been customized, make it eight spaces wide.
			if (mTabWidth == 0.0f)
				mTabWidth = mDefaultTabWidth * spaceGlyphIter->second.advance;
		}

		// Create the special glyphs. They must be created after the standard glyphs so that they take precedence in case of a
		// collision. To make sure that the indices of the special glyphs don't collide with any glyph indices in the font, we must
		// use glyph indices higher than the highest glyph index in the font.
		FT_UInt nextGlyphIndex = (FT_UInt)ftFace->num_glyphs;

		float height = (float)mDefaultHeight;

		texWidth += createGlyph(
			nextGlyphIndex++,
			GlyphInfo(static_cast<Char>(FontCodeType::Tab), 0.0f, 0.0f, mTabWidth, 0.0f, 0.0f),
			glyphHeightMap);
		texWidth += createGlyph(
			nextGlyphIndex++,
			GlyphInfo(static_cast<Char>(FontCodeType::Selected), mSelectedWidth, height, 0.0f, 0.0f, 0.0f),
			glyphHeightMap);
		texWidth += createGlyph(
			nextGlyphIndex++,
			GlyphInfo(static_cast<Char>(FontCodeType::SelectedBack), mSelectedWidth, height, 0.0f, 0.0f, 0.0f),
			glyphHeightMap);
		texWidth += createGlyph(
			nextGlyphIndex++,
			GlyphInfo(static_cast<Char>(FontCodeType::Cursor), mCursorWidth, height, 0.0f, 0.0f, 0.0f),
			glyphHeightMap);

		// If a substitute code point has been specified, check to make sure that it exists in the character map. If it doesn't,
		// revert to the default "Not Defined" code point. This is not a real code point but rather an invalid Unicode value that
		// is guaranteed to cause the "Not Defined" special glyph to be created.
		if (mSubstituteCodePoint != FontCodeType::NotDefined && mCharMap.find(mSubstituteCodePoint) == mCharMap.end())
			mSubstituteCodePoint = static_cast<Char>(FontCodeType::NotDefined);

		// Create the "Not Defined" code point (and its corresponding glyph) if it's in use as the substitute code point.
		if (mSubstituteCodePoint == FontCodeType::NotDefined)
		{
			if (!mMsdfMode)
				texWidth += createFaceGlyph(
					0,
					static_cast<Char>(FontCodeType::NotDefined),
					fontAscent,
					ftFace,
					ftLoadFlags,
					glyphHeightMap);
	#ifdef MYGUI_MSDF_FONTS
			else
				texWidth += createMsdfFaceGlyph(
					static_cast<Char>(FontCodeType::NotDefined),
					fontAscent,
					msdfFont,
					glyphHeightMap);
	#endif
		}

		// Cache a pointer to the substitute glyph info for fast lookup.
		mSubstituteGlyphInfo = &mGlyphMap.find(mSubstituteCodePoint)->second;

		// Calculate the average height of all of the glyphs that are in use. This value will be used for estimating how large the
		// texture needs to be.
		double averageGlyphHeight = 0.0;

		for (const auto& heightItem : glyphHeightMap)
			averageGlyphHeight += heightItem.first * heightItem.second.size();

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

			int texX = mGlyphSpacing;
			int texY = mGlyphSpacing;

			for (const auto& heightItem : glyphHeightMap)
			{
				for (const auto& [key, info] : heightItem.second)
				{
					int glyphWidth = (int)std::ceil(info->width);
					int glyphHeight = (int)std::ceil(info->height);

					autoWrapGlyphPos(glyphWidth, texWidth, glyphHeight, texX, texY);

					if (glyphWidth > 0)
						texX += mGlyphSpacing + glyphWidth;
				}
			}

			texHeight = Bitwise::firstPO2From(texY + glyphHeightMap.rbegin()->first);
		} while (texHeight > texWidth * 2);

		//-------------------------------------------------------------------//
		// Create the texture and render the glyphs onto it.
		//-------------------------------------------------------------------//

		if (mTexture)
		{
			RenderManager::getInstance().destroyTexture(mTexture);
			mTexture = nullptr;
		}

		mTexture = RenderManager::getInstance().createTexture(MyGUI::utility::toString((size_t)this, "_TrueTypeFont"));

		mTexture
			->createManual(texWidth, texHeight, TextureUsage::Static | TextureUsage::Write, Pixel<LAMode>::getFormat());
		mTexture->setInvalidateListener(this);

		if (!mShader.empty())
			mTexture->setShader(mShader);

		uint8* texBuffer = static_cast<uint8*>(mTexture->lock(TextureUsage::Write));

		if (texBuffer != nullptr)
		{
			// Make the texture background transparent white (or black for msdf mode).
			for (uint8 *dest = texBuffer, *endDest = dest + texWidth * texHeight * Pixel<LAMode>::getNumBytes();
				 dest != endDest;)
				Pixel<LAMode, false, false>::set(dest, mMsdfMode ? charMaskBlack : charMaskWhite, charMaskBlack, dest);

			if (!mMsdfMode)
				renderGlyphs<LAMode, Antialias>(
					glyphHeightMap,
					ftLibrary,
					ftFace,
					ftLoadFlags,
					texBuffer,
					texWidth,
					texHeight);
	#ifdef MYGUI_MSDF_FONTS
			else
				renderMsdfGlyphs(glyphHeightMap, msdfFont, texBuffer, texWidth, texHeight);
	#endif

			mTexture->unlock();

			MYGUI_LOG(
				Info,
				"ResourceTrueTypeFont: Font '"
					<< getResourceName() << "' using texture size " << texWidth << " x " << texHeight << ".");
			MYGUI_LOG(
				Info,
				"ResourceTrueTypeFont: Font '"
					<< getResourceName() << "' using real height " << mDefaultHeight << " pixels.");
		}
		else
		{
			MYGUI_LOG(Error, "ResourceTrueTypeFont: Error locking texture; pointer is nullptr.");
		}

	#ifdef MYGUI_MSDF_FONTS
		if (mMsdfMode)
		{
			msdfgen::destroyFont(msdfFont);
		}
	#endif

		FT_Done_Face(ftFace);
		FT_Done_FreeType(ftLibrary);

		delete[] fontBuffer;
	}

	FT_Face ResourceTrueTypeFont::loadFace(const FT_Library& _ftLibrary, uint8*& _fontBuffer)
	{
		FT_Face result = nullptr;

		// Load the font file.
		IDataStream* datastream = DataManager::getInstance().getData(mSource);

		if (datastream == nullptr)
			return result;

		size_t fontBufferSize = datastream->size();
		_fontBuffer = new uint8[fontBufferSize];
		datastream->read(_fontBuffer, fontBufferSize);

		DataManager::getInstance().freeData(datastream);
		datastream = nullptr;

		// Determine how many faces the font contains.
		if (FT_New_Memory_Face(_ftLibrary, _fontBuffer, (FT_Long)fontBufferSize, -1, &result) != 0)
			MYGUI_EXCEPT("ResourceTrueTypeFont: Could not load the font '" << getResourceName() << "'!");

		FT_Long numFaces = result->num_faces;
		FT_Long faceIndex = 0;

		// Load the first face.
		if (FT_New_Memory_Face(_ftLibrary, _fontBuffer, (FT_Long)fontBufferSize, faceIndex, &result) != 0)
			MYGUI_EXCEPT("ResourceTrueTypeFont: Could not load the font '" << getResourceName() << "'!");

		if (result->face_flags & FT_FACE_FLAG_SCALABLE)
		{
			// The font is scalable, so set the font size by first converting the requested size to FreeType's 26.6 fixed-point
			// format.
			FT_F26Dot6 ftSize = (FT_F26Dot6)(mSize * (1 << 6));

			if (FT_Set_Char_Size(result, ftSize, 0, mResolution, mResolution) != 0)
				MYGUI_EXCEPT("ResourceTrueTypeFont: Could not set the font size for '" << getResourceName() << "'!");

			// If no code points have been specified, use the Unicode Basic Multilingual Plane by default.
			if (mCharMap.empty())
				addCodePointRange(0, 0xFFFF);
		}
		else
		{
			// The font isn't scalable, so try to load it as a Windows FNT/FON file.
			FT_WinFNT_HeaderRec fnt;

			// Enumerate all of the faces in the font and select the smallest one that's at least as large as the requested size
			// (after adjusting for resolution). If none of the faces are large enough, use the largest one.
			std::map<float, FT_Long> faceSizes;

			do
			{
				if (FT_Get_WinFNT_Header(result, &fnt) != 0)
					MYGUI_EXCEPT("ResourceTrueTypeFont: Could not load the font '" << getResourceName() << "'!");

				faceSizes.insert(
					std::make_pair((float)fnt.nominal_point_size * fnt.vertical_resolution / mResolution, faceIndex));

				FT_Done_Face(result);

				if (++faceIndex < numFaces)
					if (FT_New_Memory_Face(_ftLibrary, _fontBuffer, (FT_Long)fontBufferSize, faceIndex, &result) != 0)
						MYGUI_EXCEPT("ResourceTrueTypeFont: Could not load the font '" << getResourceName() << "'!");
			} while (faceIndex < numFaces);

			std::map<float, FT_Long>::const_iterator iter = faceSizes.lower_bound(mSize);

			faceIndex = (iter != faceSizes.end()) ? iter->second : faceSizes.rbegin()->second;

			if (FT_New_Memory_Face(_ftLibrary, _fontBuffer, (FT_Long)fontBufferSize, faceIndex, &result) != 0)
				MYGUI_EXCEPT("ResourceTrueTypeFont: Could not load the font '" << getResourceName() << "'!");

			// Select the first bitmap strike available in the selected face. This needs to be done explicitly even though Windows
			// FNT/FON files contain only one bitmap strike per face.
			if (FT_Select_Size(result, 0) != 0)
				MYGUI_EXCEPT("ResourceTrueTypeFont: Could not set the font size for '" << getResourceName() << "'!");

			// Windows FNT/FON files do not support Unicode, so restrict the code-point range to either ISO-8859-1 or ASCII,
			// depending on the font's encoding.
			if (mCharMap.empty())
			{
				// No code points have been specified, so add the printable ASCII range by default.
				addCodePointRange(0x20, 0x7E);

				// Additionally, if the font's character set is CP-1252, add the range of non-ASCII 8-bit code points that are
				// common between CP-1252 and ISO-8859-1; i.e., everything but 0x80 through 0x9F.
				if (fnt.charset == FT_WinFNT_ID_CP1252)
					addCodePointRange(0xA0, 0xFF);
			}
			else
			{
				// Some code points have been specified, so remove anything in the non-printable ASCII range as well as anything
				// over 8 bits.
				removeCodePointRange(0, 0x1F);
				removeCodePointRange(0x100, std::numeric_limits<Char>::max());

				// Additionally, remove non-ASCII 8-bit code points (plus ASCII DEL, 0x7F). If the font's character set is CP-1252,
				// remove only the code points that differ between CP-1252 and ISO-8859-1; otherwise, remove all of them.
				if (fnt.charset == FT_WinFNT_ID_CP1252)
					removeCodePointRange(0x7F, 0x9F);
				else
					removeCodePointRange(0x7F, 0xFF);
			}
		}

		return result;
	}

	void ResourceTrueTypeFont::autoWrapGlyphPos(int _glyphWidth, int _texWidth, int _lineHeight, int& _texX, int& _texY)
		const
	{
		if (_glyphWidth > 0 && _texX + mGlyphSpacing + _glyphWidth > _texWidth)
		{
			_texX = mGlyphSpacing;
			_texY += mGlyphSpacing + _lineHeight;
		}
	}

	GlyphInfo ResourceTrueTypeFont::createFaceGlyphInfo(Char _codePoint, int _fontAscent, FT_GlyphSlot _glyph) const
	{
		float bearingX = _glyph->metrics.horiBearingX / 64.0f;

		// The following calculations aren't currently needed but are kept here for future use.
		// float ascent = _glyph->metrics.horiBearingY / 64.0f;
		// float descent = (_glyph->metrics.height / 64.0f) - ascent;

		return {
			_codePoint,
			std::max((float)_glyph->bitmap.width, _glyph->metrics.width / 64.0f),
			std::max((float)_glyph->bitmap.rows, _glyph->metrics.height / 64.0f),
			(_glyph->advance.x / 64.0f) - bearingX,
			bearingX,
			std::floor(_fontAscent - (_glyph->metrics.horiBearingY / 64.0f) - mOffsetHeight)};
	}

	int ResourceTrueTypeFont::createGlyph(
		FT_UInt _glyphIndex,
		const GlyphInfo& _glyphInfo,
		GlyphHeightMap& _glyphHeightMap)
	{
		int width = (int)std::ceil(_glyphInfo.width);
		int height = (int)std::ceil(_glyphInfo.height);

		mCharMap[_glyphInfo.codePoint] = _glyphIndex;
		GlyphInfo& info = mGlyphMap.insert(GlyphMap::value_type(_glyphInfo.codePoint, _glyphInfo)).first->second;
		_glyphHeightMap[height].insert(std::make_pair(_glyphIndex, &info));

		return (width > 0) ? mGlyphSpacing + width : 0;
	}

	int ResourceTrueTypeFont::createFaceGlyph(
		FT_UInt _glyphIndex,
		Char _codePoint,
		int _fontAscent,
		const FT_Face& _ftFace,
		FT_Int32 _ftLoadFlags,
		GlyphHeightMap& _glyphHeightMap)
	{
		if (mGlyphMap.find(_codePoint) == mGlyphMap.end())
		{
			if (FT_Load_Glyph(_ftFace, _glyphIndex, _ftLoadFlags) == 0)
				return createGlyph(
					_glyphIndex,
					createFaceGlyphInfo(_codePoint, _fontAscent, _ftFace->glyph),
					_glyphHeightMap);
			MYGUI_LOG(
				Warning,
				"ResourceTrueTypeFont: Cannot load glyph "
					<< _glyphIndex << " for character " << _codePoint << " in font '" << getResourceName() << "'.");
		}
		else
		{
			mCharMap[_codePoint] = _glyphIndex;
		}

		return 0;
	}

	template<bool LAMode, bool Antialias>
	void ResourceTrueTypeFont::renderGlyphs(
		const GlyphHeightMap& _glyphHeightMap,
		const FT_Library& _ftLibrary,
		const FT_Face& _ftFace,
		FT_Int32 _ftLoadFlags,
		uint8* _texBuffer,
		int _texWidth,
		int _texHeight)
	{
		FT_Bitmap ftBitmap;
		FT_Bitmap_New(&ftBitmap);

		int texX = mGlyphSpacing;
		int texY = mGlyphSpacing;

		for (const auto& sameHeightGlyphs : _glyphHeightMap)
		{
			int glyphHeight = sameHeightGlyphs.first;
			for (const auto& glyph : sameHeightGlyphs.second)
			{
				GlyphInfo& info = *glyph.second;

				switch (info.codePoint)
				{
				case FontCodeType::Selected:
				case FontCodeType::SelectedBack:
				{
					renderGlyph<LAMode, false, false>(
						info,
						charMaskWhite,
						charMaskBlack,
						charMask.find(info.codePoint)->second,
						glyphHeight,
						_texBuffer,
						_texWidth,
						_texHeight,
						texX,
						texY);

					// Manually adjust the glyph's width to zero. This prevents artifacts from appearing at the seams when
					// rendering multi-character selections.
					GlyphMap::iterator glyphIter = mGlyphMap.find(info.codePoint);
					if (glyphIter != mGlyphMap.end())
					{
						glyphIter->second.width = 0.0f;
						glyphIter->second.uvRect.right = glyphIter->second.uvRect.left;
					}
				}
				break;

				case FontCodeType::Cursor:
				case FontCodeType::Tab:
					renderGlyph<LAMode, false, false>(
						info,
						charMaskWhite,
						charMaskBlack,
						charMask.find(info.codePoint)->second,
						glyphHeight,
						_texBuffer,
						_texWidth,
						_texHeight,
						texX,
						texY);
					break;

				default:
					if (FT_Load_Glyph(_ftFace, glyph.first, _ftLoadFlags | FT_LOAD_RENDER) == 0)
					{
						if (_ftFace->glyph->bitmap.buffer != nullptr)
						{
							uint8* glyphBuffer = nullptr;

							switch (_ftFace->glyph->bitmap.pixel_mode)
							{
							case FT_PIXEL_MODE_GRAY: glyphBuffer = _ftFace->glyph->bitmap.buffer; break;

							case FT_PIXEL_MODE_MONO:
								// Convert the monochrome bitmap to 8-bit before rendering it.
								if (FT_Bitmap_Convert(_ftLibrary, &_ftFace->glyph->bitmap, &ftBitmap, 1) == 0)
								{
									// Go through the bitmap and convert all of the nonzero values to 0xFF (white).
									for (uint8 *p = ftBitmap.buffer, *endP = p + ftBitmap.width * ftBitmap.rows;
										 p != endP;
										 ++p)
										*p = *p ? 0xFF : 0;

									glyphBuffer = ftBitmap.buffer;
								}
								break;
							}

							if (glyphBuffer != nullptr)
								renderGlyph<LAMode, true, Antialias>(
									info,
									charMaskWhite,
									charMaskWhite,
									charMaskWhite,
									glyphHeight,
									_texBuffer,
									_texWidth,
									_texHeight,
									texX,
									texY,
									glyphBuffer);
						}
					}
					else
					{
						MYGUI_LOG(
							Warning,
							"ResourceTrueTypeFont: Cannot render glyph "
								<< glyph.first << " for character " << info.codePoint << " in font '"
								<< getResourceName() << "'.");
					}
					break;
				}
			}
		}

		FT_Bitmap_Done(_ftLibrary, &ftBitmap);
	}

	template<bool LAMode, bool UseBuffer, bool Antialias>
	void ResourceTrueTypeFont::renderGlyph(
		GlyphInfo& _info,
		uint8 _luminance0,
		uint8 _luminance1,
		uint8 _alpha,
		int _lineHeight,
		uint8* _texBuffer,
		int _texWidth,
		int _texHeight,
		int& _texX,
		int& _texY,
		uint8* _glyphBuffer)
	{
		int width = (int)std::ceil(_info.width);
		int height = (int)std::ceil(_info.height);

		autoWrapGlyphPos(width, _texWidth, _lineHeight, _texX, _texY);

		uint8* dest = _texBuffer + (_texY * _texWidth + _texX) * Pixel<LAMode>::getNumBytes();

		// Calculate how much to advance the destination pointer after each row to get to the start of the next row.
		ptrdiff_t destNextRow = (_texWidth - width) * Pixel<LAMode>::getNumBytes();

		if (!mMsdfMode || !UseBuffer)
		{
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
		}
		else
		{
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					for (int i = 0; i < 3; ++i)
					{
						*dest++ = *_glyphBuffer++;
					}
					*dest++ = 255;
				}

				dest += destNextRow;
			}
		}

		// Calculate and store the glyph's UV coordinates within the texture.
		_info.uvRect.left = (float)_texX / _texWidth; // u1
		_info.uvRect.top = (float)_texY / _texHeight; // v1
		_info.uvRect.right = (float)(_texX + _info.width) / _texWidth; // u2
		_info.uvRect.bottom = (float)(_texY + _info.height) / _texHeight; // v2

		if (width > 0)
			_texX += mGlyphSpacing + width;
	}

	#ifdef MYGUI_MSDF_FONTS
	GlyphInfo ResourceTrueTypeFont::createMsdfFaceGlyphInfo(
		Char _codePoint,
		const msdfgen::Shape& _shape,
		double _advance,
		int _fontAscent)
	{
		msdfgen::Shape::Bounds bounds = _shape.getBounds();
		double range = mMsdfRange / 2.0;
		if (_shape.contours.empty())
		{
			bounds = {0, 0, 0, 0};
			range = 0;
		}

		double bearingX = bounds.l;

		return GlyphInfo(
			_codePoint,
			bounds.r - bounds.l + 2 * range,
			bounds.t - bounds.b + 2 * range,
			_advance - bearingX + range,
			bearingX - range,
			std::floor(_fontAscent - bounds.t - mOffsetHeight - range));
	}

	int ResourceTrueTypeFont::createMsdfGlyph(const GlyphInfo& _glyphInfo, GlyphHeightMap& _glyphHeightMap)
	{
		int width = (int)std::ceil(_glyphInfo.width);
		int height = (int)std::ceil(_glyphInfo.height);

		mCharMap[_glyphInfo.codePoint] = _glyphInfo.codePoint;
		GlyphInfo& info = mGlyphMap.insert(GlyphMap::value_type(_glyphInfo.codePoint, _glyphInfo)).first->second;
		_glyphHeightMap[height].insert(std::make_pair(_glyphInfo.codePoint, &info));

		return (width > 0) ? mGlyphSpacing + width : 0;
	}

	int ResourceTrueTypeFont::createMsdfFaceGlyph(
		Char _codePoint,
		int _fontAscent,
		msdfgen::FontHandle* _fontHandle,
		GlyphHeightMap& _glyphHeightMap)
	{
		if (mGlyphMap.find(_codePoint) == mGlyphMap.end())
		{
			msdfgen::Shape shape;
			double advance;
			if (msdfgen::loadGlyph(shape, _fontHandle, _codePoint, &advance))
				createMsdfGlyph(createMsdfFaceGlyphInfo(_codePoint, shape, advance, _fontAscent), _glyphHeightMap);
			else
				MYGUI_LOG(
					Warning,
					"ResourceTrueTypeFont: Cannot load msdf glyph for character "
						<< _codePoint << " in font '" << getResourceName() << "'.");
		}
		else
		{
			mCharMap[_codePoint] = _codePoint;
		}

		return 0;
	}

	void ResourceTrueTypeFont::renderMsdfGlyphs(
		const GlyphHeightMap& _glyphHeightMap,
		msdfgen::FontHandle* _fontHandle,
		uint8* _texBuffer,
		int _texWidth,
		int _texHeight)
	{
		int texX = mGlyphSpacing, texY = mGlyphSpacing;

		for (const auto& sameHeightGlyphs : _glyphHeightMap)
		{
			int glyphHeight = sameHeightGlyphs.first;
			for (const auto& glyph : sameHeightGlyphs.second)
			{
				GlyphInfo& info = *glyph.second;

				switch (info.codePoint)
				{
				case FontCodeType::Selected:
				case FontCodeType::SelectedBack:
				{
					renderGlyph<false, false, false>(
						info,
						charMaskWhite,
						charMaskBlack,
						charMask.find(info.codePoint)->second,
						glyphHeight,
						_texBuffer,
						_texWidth,
						_texHeight,
						texX,
						texY);

					// Manually adjust the glyph's width to zero. This prevents artifacts from appearing at the seams when
					// rendering multi-character selections.
					GlyphMap::iterator glyphIter = mGlyphMap.find(info.codePoint);
					if (glyphIter != mGlyphMap.end())
					{
						glyphIter->second.width = 0.0f;
						glyphIter->second.uvRect.right = glyphIter->second.uvRect.left;
					}
				}
				break;

				case FontCodeType::Cursor:
				case FontCodeType::Tab:
					renderGlyph<false, false, false>(
						info,
						charMaskWhite,
						charMaskBlack,
						charMask.find(info.codePoint)->second,
						glyphHeight,
						_texBuffer,
						_texWidth,
						_texHeight,
						texX,
						texY);
					break;

				default:
					msdfgen::Shape shape;
					if (loadGlyph(shape, _fontHandle, info.codePoint))
					{
						msdfgen::Shape::Bounds bounds = shape.getBounds();
						double range = mMsdfRange / 2.0;
						if (shape.contours.empty())
						{
							bounds = {0, 0, 0, 0};
							range = 0;
						}

						shape.normalize();
						edgeColoringSimple(shape, 3.0);

						msdfgen::Bitmap<float, 3> msdf(
							std::ceil(bounds.r - bounds.l + 2 * range),
							std::ceil(bounds.t - bounds.b + 2 * range));
						msdfgen::generateMSDF(
							msdf,
							shape,
							mMsdfRange,
							1,
							msdfgen::Vector2(-bounds.l + range, -bounds.b + range));
						//							double error = msdfgen::estimateSDFError(
						//								msdfgen::BitmapConstRef<float, 3>{(float*)msdf, msdf.width(), msdf.height()},
						//								shape,
						//								1,
						//								msdfgen::Vector2(-bounds.l + range, -bounds.b + range),
						//								33);
						//							if (100000 * error > 1)
						//								MYGUI_LOG(Warning, "Error for '" << char(info.codePoint) << "' is :" << (int) 100000 * error);

						uint8* glyphBuffer = new uint8[msdf.width() * msdf.height() * 3];
						uint8* glyphBufferPointer = glyphBuffer;
						for (int y = 0; y < msdf.height(); ++y)
						{
							for (int x = 0; x < msdf.width(); ++x)
							{
								for (int i = 0; i < 3; ++i)
								{
									// upside-down and RGB->BGR
									*glyphBufferPointer++ =
										msdfgen::pixelFloatToByte(msdf(x, msdf.height() - y - 1)[2 - i]);
								}
							}
						}

						renderGlyph<false, true, false>(
							info,
							charMaskWhite,
							charMaskWhite,
							charMaskWhite,
							glyphHeight,
							_texBuffer,
							_texWidth,
							_texHeight,
							texX,
							texY,
							glyphBuffer);
						delete[] glyphBuffer;
					}
					else
					{
						MYGUI_LOG(
							Warning,
							"ResourceTrueTypeFont: Cannot render glyph for character "
								<< info.codePoint << " in font '" << getResourceName() << "'.");
					}
					break;
				}
			}
		}
	}
	#endif

	void ResourceTrueTypeFont::setSource(std::string_view _value)
	{
		mSource = _value;
	}

	void ResourceTrueTypeFont::setShader(std::string_view _value)
	{
		mShader = _value;
	}

	void ResourceTrueTypeFont::setSize(float _value)
	{
		mSize = _value;
	}

	void ResourceTrueTypeFont::setResolution(unsigned int _value)
	{
		mResolution = _value;
	}

	void ResourceTrueTypeFont::setHinting(std::string_view _value)
	{
		if (_value == "use_native")
			mHinting = HintingUseNative;
		else if (_value == "force_auto")
			mHinting = HintingForceAuto;
		else if (_value == "disable_auto")
			mHinting = HintingDisableAuto;
		else if (_value == "disable_all")
			mHinting = HintingDisableAll;
		else
			mHinting = HintingUseNative;
	}

	void ResourceTrueTypeFont::setAntialias(bool _value)
	{
		mAntialias = _value;
	}

	void ResourceTrueTypeFont::setTabWidth(float _value)
	{
		mTabWidth = _value;
	}

	void ResourceTrueTypeFont::setOffsetHeight(int _value)
	{
		mOffsetHeight = _value;
	}

	void ResourceTrueTypeFont::setSubstituteCode(int _value)
	{
		mSubstituteCodePoint = _value;
	}

	void ResourceTrueTypeFont::setDistance(int _value)
	{
		mGlyphSpacing = _value;
	}

	void ResourceTrueTypeFont::setMsdfMode(bool _value)
	{
	#ifndef MYGUI_MSDF_FONTS
		if (_value)
			MYGUI_LOG(
				Error,
				"MsdfMode flag ignored Define MYGUI_MSDF_FONTS if you need msdf fonts, msdf mode ignored.");
	#else
		mMsdfMode = _value;
	#endif
	}

	void ResourceTrueTypeFont::setMsdfRange(int _value)
	{
		mMsdfRange = _value;
	}

#endif // MYGUI_USE_FREETYPE

} // namespace MyGUI
