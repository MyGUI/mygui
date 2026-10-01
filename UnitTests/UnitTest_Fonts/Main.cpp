#include "FontTestContext.h"
#include "TestRunner.h"
#include <algorithm>
#include <cmath>
#include <set>

namespace
{

	using unittest::require;
	using Font = MyGUI::ResourceTrueTypeFont;
#ifdef MYGUI_UNITTEST_MSDF
	constexpr bool msdf = true;
#else
	constexpr bool msdf = false;
#endif

	void near(float _actual, float _expected, const char* _message, float _tolerance = 0.001f)
	{
		require(std::isfinite(_actual) && std::abs(_actual - _expected) <= _tolerance, _message);
	}

	// Go through the resource factory and XML property handling used by applications.
	Font& loadFont(
		std::string_view _properties = {},
		std::string_view _codes = "<Code range=\"63 90\"/><Code range=\"103\"/>")
	{
		static unsigned int sequence = 0;
		const auto name = "TestFont" + std::to_string(++sequence);
		std::stringstream xml;
		xml << "<MyGUI type=\"Resource\"><Resource type=\"ResourceTrueTypeFont\" name=\"" << name << "\">"
			<< "<Property key=\"Source\" value=\"DejaVuSans.ttf\"/>"
			<< "<Property key=\"Size\" value=\"24\"/><Property key=\"Resolution\" value=\"96\"/>"
			<< "<Property key=\"AutoDpi\" value=\"false\"/>"
			<< "<Property key=\"MsdfMode\" value=\"" << (msdf ? "true" : "false") << "\"/>" << _properties << "<Codes>"
			<< _codes << "</Codes></Resource></MyGUI>";
		MyGUI::xml::Document document;
		require(document.open(xml), "Font test XML must parse");
		MyGUI::ResourceManager::getInstance().loadFromXmlNode(document.getRoot(), "", MyGUI::Version(1, 1));
		auto* font = MyGUI::FontManager::getInstance().getByName(name);
		require(font != nullptr && font->isType<Font>(), "XML must register a TrueType font with FontManager");
		return *font->castType<Font>();
	}

	const MyGUI::GlyphInfo& glyph(const Font& _font, MyGUI::Char _code)
	{
		const auto* result = _font.getGlyphInfo(_code);
		require(result != nullptr && result->codePoint == _code, "Requested glyph must exist without substitution");
		return *result;
	}

	unittest::FontTexture& atlas(const Font& _font)
	{
		auto* texture = _font.getTextureFont();
		require(texture != nullptr, "Initialised font must expose an atlas");
		require(!texture->isLocked(), "Font must unlock its atlas after upload");
		return *static_cast<unittest::FontTexture*>(texture);
	}

	bool contains(const Font& _font, MyGUI::Char _code)
	{
		const auto ranges = _font.getCodePointRanges();
		return std::any_of(
			ranges.begin(),
			ranges.end(),
			[&](const auto& _range) { return _range.first <= _code && _code <= _range.second; });
	}

	void testRangeEditing()
	{
		Font font;
		font.addCodePointRange(65, 70);
		font.addCodePointRange(68, 74);
		font.addCodePointRange(80, 80);
		font.removeCodePointRange(67, 69);
		font.removeCodePointRange(200, 210);
		using Ranges = std::vector<std::pair<MyGUI::Char, MyGUI::Char>>;
		require(
			font.getCodePointRanges() == Ranges{{65, 66}, {70, 74}, {80, 80}},
			"Overlapping inclusions must merge and inclusive exclusions must split ranges");
	}

	void testFilteringAndSubstitution()
	{
		unittest::FontTestContext context;
		auto& font = loadFont(
			"<Property key=\"SubstituteCode\" value=\"63\"/>",
			"<Code hide=\"66 67\"/><Code range=\"63\"/><Code range=\"65 68\"/><Code range=\"1114111\"/>");
		glyph(font, 'A');
		glyph(font, 'D');
		require(
			!contains(font, 'B') && !contains(font, 'C') && !contains(font, 0x10FFFF),
			"XML exclusions and unsupported code points must be removed from supported ranges");
		require(font.getSubstituteCodePoint() == '?', "XML must select the requested substitute");
		for (MyGUI::Char code : std::initializer_list<MyGUI::Char>{'B', 'C', 'Z', 0x10FFFF})
			require(
				font.getGlyphInfo(code) == &glyph(font, '?'),
				"Excluded, unrequested and unsupported characters must use the configured substitute");
	}

	void testDefaultSubstitute()
	{
		unittest::FontTestContext context;
		for (const char* property : {"", "<Property key=\"SubstituteCode\" value=\"90\"/>"})
		{
			auto& font = loadFont(property, "<Code range=\"65\"/>");
			require(
				font.getSubstituteCodePoint() == MyGUI::FontCodeType::NotDefined,
				"Missing custom substitute must fall back to NotDefined");
			require(
				font.getGlyphInfo('Z') == &glyph(font, MyGUI::FontCodeType::NotDefined),
				"Unloaded characters must resolve to the default substitute");
		}
	}

	void testSpecialGlyphs()
	{
		unittest::FontTestContext context;
		auto& font = loadFont({}, "<Code range=\"65\"/>");
		const auto& space = glyph(font, MyGUI::FontCodeType::Space);
		const auto& tab = glyph(font, MyGUI::FontCodeType::Tab);
		require(
			space.advance > 0 && space.width == 0 && space.height == 0,
			"Omitted space must be inserted with an advance and no visible rectangle");
		near(tab.advance, 8 * space.advance, "Default tab must advance eight spaces");
		require(tab.width == 0 && tab.height == 0, "Tab must not render visible pixels");
		for (auto code : {MyGUI::FontCodeType::Selected, MyGUI::FontCodeType::SelectedBack})
		{
			const auto& selection = glyph(font, code);
			require(
				selection.advance == 0 && selection.width == 0 && selection.height > 0,
				"Selection must not affect layout or introduce seams");
			near(selection.uvRect.left, selection.uvRect.right, "Selection must sample a single atlas column");
		}
		const auto& cursor = glyph(font, MyGUI::FontCodeType::Cursor);
		require(
			cursor.width == 2 && cursor.height == font.getDefaultHeight() && cursor.advance == 0,
			"Cursor must span the line height without advancing text");
		auto& custom = loadFont("<Property key=\"TabWidth\" value=\"19.5\"/>");
		near(glyph(custom, MyGUI::FontCodeType::Tab).advance, 19.5f, "XML TabWidth must override automatic width");
	}

	void testLayoutProperties()
	{
		unittest::FontTestContext context;
		auto& normal = loadFont();
		auto& shifted = loadFont("<Property key=\"OffsetHeight\" value=\"3\"/>");
		auto& sparse = loadFont({}, "<Code range=\"65\"/>");
		require(
			normal.getDefaultHeight() > 0 && normal.getDefaultHeight() == sparse.getDefaultHeight(),
			"Line height must be independent of the requested glyph set");
		const auto& a = glyph(normal, 'A');
		const auto& b = glyph(shifted, 'A');
		near(b.bearingY, a.bearingY - 3, "OffsetHeight must move the glyph vertically");
		near(b.advance, a.advance, "OffsetHeight must preserve advance");
		near(b.width, a.width, "OffsetHeight must preserve width");
		require(normal.getDefaultHeight() == shifted.getDefaultHeight(), "OffsetHeight must preserve line height");
	}

	void testKerning()
	{
		unittest::FontTestContext context;
		auto& enabled = loadFont();
		require(enabled.getKerning('A', 'V') < 0, "MyGUI must expose the bundled font's AV kerning");
		auto& disabled = loadFont("<Property key=\"Kerning\" value=\"false\"/>");
		near(disabled.getKerning('A', 'V'), 0, "Kerning=false must suppress pair adjustments");
		near(glyph(disabled, 'A').advance, glyph(enabled, 'A').advance, "Kerning toggle must preserve glyph advance");
		auto& excluded = loadFont({}, "<Code range=\"65\"/>");
		near(excluded.getKerning('A', 'V'), 0, "Kerning must not include excluded glyphs");
		near(enabled.getKerning(0x10FFFF, 'V'), 0, "Missing pairs must have zero kerning");
	}

	void testAtlasAndLifetime()
	{
		unittest::FontTestContext context;
		auto& font = loadFont("<Property key=\"Shader\" value=\"FontTestShader\"/>");
		auto& texture = atlas(font);
		require(texture.shader == "FontTestShader", "XML Shader must be passed to the atlas");
		for (MyGUI::Char code : {'A', 'V', 'g', '?'})
		{
			const auto& g = glyph(font, code);
			require(g.width > 0 && g.height > 0 && g.advance > 0, "Visible glyph must have usable metrics");
			const auto& uv = g.uvRect;
			require(
				uv.left >= 0 && uv.top >= 0 && uv.right <= 1 && uv.bottom <= 1 && uv.left < uv.right &&
					uv.top < uv.bottom,
				"Glyph UVs must stay inside the atlas");
			near((uv.right - uv.left) * texture.getWidth(), g.width, "UV width must match glyph width");
			near((uv.bottom - uv.top) * texture.getHeight(), g.height, "UV height must match glyph height");
		}
		const auto pixels = texture.pixels;
		const auto before = glyph(font, 'A');
		const auto ranges = font.getCodePointRanges();
		const auto kerning = font.getKerning('A', 'V');
		require(texture.listener != nullptr, "Atlas must register for texture invalidation");
		// The callback destroys the old texture, so retain no references to it afterwards.
		texture.listener->textureInvalidate(&texture);
		require(
			context.renderer.created == 2 && context.renderer.destroyed == 1,
			"Invalidation must replace and release the old atlas");
		require(
			atlas(font).pixels == pixels && atlas(font).shader == "FontTestShader",
			"Invalidation must restore atlas content and shader");
		near(glyph(font, 'A').advance, before.advance, "Invalidation must preserve layout metrics");
		require(
			glyph(font, 'A').uvRect == before.uvRect && font.getCodePointRanges() == ranges,
			"Invalidation must restore glyph UVs and supported ranges");
		near(font.getKerning('A', 'V'), kerning, "Invalidation must restore kerning");
		require(
			font.getGlyphInfo(0x10FFFF) == &glyph(font, font.getSubstituteCodePoint()),
			"Invalidation must reconnect the substitute glyph");
		const auto name = font.getResourceName();
		require(MyGUI::ResourceManager::getInstance().removeByName(name), "Font resource must be removable");
		require(
			context.renderer.textures.empty() && context.renderer.destroyed == 2,
			"Removing the resource must release its atlas");
	}

	void testMissingSource()
	{
		unittest::FontTestContext context;
		auto& font = loadFont("<Property key=\"Source\" value=\"missing.ttf\"/>");
		require(
			font.getTextureFont() == nullptr && font.getGlyphInfo('A') == nullptr && font.getDefaultHeight() == 0,
			"Missing font data must leave the font empty");
		require(context.renderer.created == 0, "Missing source must not allocate an atlas");
	}

	void testDpi()
	{
		unittest::FontTestContext context;
		auto& normal = loadFont();
		auto& scaled = loadFont("<Property key=\"DpiScale\" value=\"2\"/>");
		context.gui.setDpiScale(2);
		auto& automatic =
			loadFont("<Property key=\"AutoDpi\" value=\"true\"/><Property key=\"DpiScale\" value=\"3\"/>");
		auto& explicitScale = loadFont();
		require(
			atlas(automatic).pixels == atlas(scaled).pixels,
			"AutoDpi must select the GUI scale instead of the explicit scale");
		require(atlas(explicitScale).pixels == atlas(normal).pixels, "AutoDpi=false must ignore the GUI scale");
		const auto& a = glyph(normal, 'A');
		const auto& b = glyph(scaled, 'A');
		if (msdf)
		{
			require(
				atlas(normal).pixels == atlas(scaled).pixels,
				"MSDF generation must ignore DPI scaling and reuse the same atlas resolution");
			near(b.advance, a.advance, "MSDF advance must not depend on DPI");
		}
		else
		{
			// Hinting and rounding may move logical metrics by up to one pixel.
			near(b.advance, a.advance, "Higher DPI must preserve logical advance", 1);
			near(
				float(scaled.getDefaultHeight()),
				float(normal.getDefaultHeight()),
				"Higher DPI must preserve logical line height",
				1);
			const float pixelWidth = (b.uvRect.right - b.uvRect.left) * atlas(scaled).getWidth();
			near(pixelWidth, b.width * 2, "Higher DPI must allocate twice the pixels per logical glyph unit");
			require(pixelWidth > a.width * 1.5f, "DPI setting must actually increase raster resolution");
		}
	}


	void testTextLayout()
	{
		unittest::FontTestContext context;
		auto& font = loadFont("<Property key=\"SubstituteCode\" value=\"63\"/>");
		auto* text = unittest::createFontTextBox(context.gui, font);
		const int height = font.getDefaultHeight();
		const float a = glyph(font, 'A').advance;
		const float v = glyph(font, 'V').advance;
		const float av = a + v + font.getKerning('A', 'V');
		auto measure = [&](const MyGUI::UString& caption)
		{
			text->setCaption(caption);
			return text->getTextSize();
		};
		require(text->getFontHeight() == height, "FontHeight=0 must use the generated font's line height");
		require(
			measure("AV") == MyGUI::IntSize(int(std::ceil(av)), height),
			"TextBox width must include generated glyph advances and pair kerning");
		auto& unkerned = loadFont("<Property key=\"Kerning\" value=\"false\"/>");
		text->setFontName(unkerned.getResourceName());
		require(
			measure("AV").width == int(std::ceil(a + v)) && measure("AV").width > int(std::ceil(av)),
			"Switching fonts must refresh layout and remove the disabled kerning adjustment");
		text->setFontName(font.getResourceName());
		const float tabbed = a + glyph(font, MyGUI::FontCodeType::Tab).advance + v;
		require(
			measure("A\tV") == MyGUI::IntSize(int(std::ceil(tabbed)), height),
			"TextBox must use the generated tab advance and break the AV kerning pair");
		for (const char* caption : {"A\nV", "A\r\nV"})
			require(
				measure(caption) == MyGUI::IntSize(int(std::ceil(std::max(a, v))), 2 * height),
				"Line breaks must reset kerning and add one font height");
		require(
			measure("AV\n") == MyGUI::IntSize(int(std::ceil(av)), 2 * height),
			"A trailing newline must retain an empty line at the generated font height");
		const auto substituteSize = measure("?");
		require(
			measure(MyGUI::UString(0x10FFFF)) == substituteSize && substituteSize.width > 0,
			"A missing character must occupy the configured substitute's advance");
		text->setFontHeight(2 * height);
		require(
			measure("AV") == MyGUI::IntSize(int(std::ceil(2 * av)), 2 * height),
			"Custom text height must scale both glyph advances and kerning");
	}

	template<bool Scaled>
	void testAtlasPacking()
	{
		unittest::FontTestContext context;
		constexpr int distance = Scaled ? 4 : 1;
		auto& font = loadFont(
			Scaled ? "<Property key=\"Size\" value=\"31.5\"/><Property key=\"Distance\" value=\"4\"/>"
					 "<Property key=\"DpiScale\" value=\"1.5\"/><Property key=\"MsdfRange\" value=\"6\"/>"
				   : "",
			"<Code range=\"33 126\"/>");
		auto& texture = atlas(font);
		std::vector<MyGUI::IntRect> rectangles;
		std::set<int> rows;
		std::set<int> heights;
		// ASCII avoids aliases sharing a font glyph. Include MyGUI's visible synthetic glyphs too.
		std::vector<MyGUI::Char> codes;
		for (MyGUI::Char code = 33; code <= 126; ++code)
			codes.push_back(code);
		codes.push_back(MyGUI::FontCodeType::Cursor);
		codes.push_back(MyGUI::FontCodeType::NotDefined);
		for (auto code : codes)
		{
			const auto& uv = glyph(font, code).uvRect;
			const int left = int(std::lround(uv.left * texture.getWidth()));
			const int top = int(std::lround(uv.top * texture.getHeight()));
			const int width = int(std::ceil((uv.right - uv.left) * texture.getWidth() - 0.001f));
			const int height = int(std::ceil((uv.bottom - uv.top) * texture.getHeight() - 0.001f));
			const MyGUI::IntRect rectangle(left, top, left + width, top + height);
			require(width > 0 && height > 0, "Every requested visible glyph must have an atlas rectangle");
			require(
				left >= 0 && top >= 0 && rectangle.right <= texture.getWidth() &&
					rectangle.bottom <= texture.getHeight(),
				"Packed glyph rectangles must remain inside the atlas");
			for (const auto& other : rectangles)
				require(
					rectangle.right + distance <= other.left || other.right + distance <= rectangle.left ||
						rectangle.bottom + distance <= other.top || other.bottom + distance <= rectangle.top,
					"Packed glyphs must not overlap and must preserve the configured spacing");
			bool hasInk = false;
			const size_t stride = texture.getNumElemBytes();
			for (int y = top; y < rectangle.bottom; ++y)
				for (int x = left; x < rectangle.right; ++x)
					hasInk |= texture.pixels[(size_t(y) * texture.getWidth() + x) * stride + stride - 1] != 0;
			require(hasInk, "Every packed glyph must point at uploaded pixels, including after row wrapping");
			rectangles.push_back(rectangle);
			rows.insert(top);
			heights.insert(height);
		}
		require(
			rows.size() >= 3 && heights.size() >= 3,
			"Packing fixture must exercise several atlas rows and mixed glyph heights");
	}

	template<bool LuminanceAlpha, bool Antialias>
	void testBitmapUpload()
	{
		unittest::FontTestContext context;
		context.renderer.supportsLuminanceAlpha = LuminanceAlpha;
		auto& font = loadFont(Antialias ? "<Property key=\"Antialias\" value=\"true\"/>" : "");
		auto& texture = atlas(font);
		require(
			texture.getFormat() == (LuminanceAlpha ? MyGUI::PixelFormat::L8A8 : MyGUI::PixelFormat::R8G8B8A8),
			"Bitmap fonts must prefer L8A8 with an RGBA fallback");
		const auto& g = glyph(font, 'A');
		const int left = int(std::lround(g.uvRect.left * texture.getWidth()));
		const int top = int(std::lround(g.uvRect.top * texture.getHeight()));
		bool hasInk = false;
		const auto stride = texture.getNumElemBytes();
		for (int y = top; y < top + int(g.height); ++y)
			for (int x = left; x < left + int(g.width); ++x)
			{
				const auto* pixel = &texture.pixels[(size_t(y) * texture.getWidth() + x) * stride];
				const auto alpha = pixel[stride - 1];
				hasInk |= alpha != 0;
				for (size_t channel = 0; channel < stride - 1; ++channel)
					require(
						pixel[channel] == (Antialias ? alpha : 255),
						"MyGUI must copy coverage to luminance only when Antialias is enabled");
			}
		require(hasInk, "MyGUI must upload visible glyph coverage");
	}

	void testMsdfRange()
	{
		unittest::FontTestContext context;
		auto& narrow = loadFont("<Property key=\"MsdfRange\" value=\"2\"/>");
		auto& wide = loadFont("<Property key=\"MsdfRange\" value=\"6\"/>");
		const auto& a = glyph(narrow, 'A');
		const auto& b = glyph(wide, 'A');
		near(b.width, a.width + 4, "MSDF range must expand glyph width by the added padding");
		near(b.height, a.height + 4, "MSDF range must expand glyph height by the added padding");
		near(b.bearingX, a.bearingX - 2, "MSDF padding must shift the left bearing");
		near(b.bearingY, a.bearingY - 2, "MSDF padding must shift the top bearing");
		near(b.advance, a.advance, "MSDF range must preserve text advance");
		require(narrow.getDefaultHeight() == wide.getDefaultHeight(), "MSDF range must preserve line spacing");
		const auto& space = glyph(wide, MyGUI::FontCodeType::Space);
		require(space.width == 0 && space.height == 0, "MSDF range must not give empty glyphs visible padding");
	}

	void testMsdfUpload()
	{
		unittest::FontTestContext context;
		auto& font = loadFont();
		auto& texture = atlas(font);
		require(
			texture.getFormat() == MyGUI::PixelFormat::R8G8B8A8,
			"MSDF must use RGBA even when the renderer supports L8A8");
		require(
			std::all_of(texture.pixels.begin(), texture.pixels.begin() + 4, [](auto b) { return b == 0; }),
			"MSDF atlas background must be transparent black");
		const auto& g = glyph(font, 'A');
		const int left = int(std::lround(g.uvRect.left * texture.getWidth()));
		const int top = int(std::lround(g.uvRect.top * texture.getHeight()));
		bool hasColour = false;
		for (int y = top; y < top + int(std::ceil(g.height)); ++y)
			for (int x = left; x < left + int(std::ceil(g.width)); ++x)
			{
				const auto* pixel = &texture.pixels[(size_t(y) * texture.getWidth() + x) * 4];
				require(pixel[3] == 255, "MyGUI must write opaque alpha for MSDF glyph rectangles");
				hasColour |= pixel[0] != pixel[1] || pixel[1] != pixel[2];
			}
		require(hasColour, "MyGUI must retain the separate distance channels when uploading MSDF data");
	}

}

int main()
{
	std::vector<unittest::TestCase> tests{
		{"code point range editing", testRangeEditing},
		{"XML filtering and custom substitution", testFilteringAndSubstitution},
		{"default and unavailable substitutes", testDefaultSubstitute},
		{"space, tab, selection and cursor glyphs", testSpecialGlyphs},
		{"line height and vertical offset", testLayoutProperties},
		{"kerning configuration and filtering", testKerning},
		{"atlas upload, invalidation and resource lifetime", testAtlasAndLifetime},
		{"missing font source", testMissingSource},
		{"explicit and automatic DPI", testDpi},
		{"generated fonts in TextBox layout", testTextLayout},
		{"atlas packing across rows", testAtlasPacking<false>},
		{"atlas packing with larger spacing and scale", testAtlasPacking<true>},
	};
	if (msdf)
	{
		tests.push_back({"MSDF range and padding", testMsdfRange});
		tests.push_back({"MSDF texture format and channel upload", testMsdfUpload});
	}
	else
	{
		tests.push_back({"L8A8 antialias upload", testBitmapUpload<true, true>});
		tests.push_back({"L8A8 coverage upload", testBitmapUpload<true, false>});
		tests.push_back({"RGBA antialias upload", testBitmapUpload<false, true>});
		tests.push_back({"RGBA coverage upload", testBitmapUpload<false, false>});
	}
	return unittest::runTests(tests);
}
