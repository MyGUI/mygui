#include "FontTestContext.h"
#include "TestRunner.h"
#include "MyGUI_ResourceManualFont.h"

namespace
{

	using unittest::require;
	using Font = MyGUI::ResourceManualFont;

	unittest::FontTexture& createAtlas(unittest::FontTestContext& _context, const std::string& _name = "ManualAtlas")
	{
		auto* texture = static_cast<unittest::FontTexture*>(_context.renderer.createTexture(_name));
		texture->createManual(128, 64, MyGUI::TextureUsage::Static, MyGUI::PixelFormat::R8G8B8A8);
		return *texture;
	}

	Font& loadFont(std::string_view _codes, MyGUI::Version _version = MyGUI::Version(1, 2))
	{
		static unsigned int sequence = 0;
		const auto name = "ManualFont" + std::to_string(++sequence);
		unittest::loadFontTestResources(
			"<MyGUI type=\"Resource\"><Resource type=\"ResourceManualFont\" name=\"" + name +
				"\">"
				"<Property key=\"Source\" value=\"ManualAtlas\"/>"
				"<Property key=\"DefaultHeight\" value=\"20\"/>"
				"<Property key=\"Shader\" value=\"ManualFontShader\"/>"
				"<Codes>" +
				std::string(_codes) + "</Codes></Resource></MyGUI>",
			_version);
		auto* font = MyGUI::FontManager::getInstance().getByName(name);
		require(font != nullptr && font->isType<Font>(), "XML must register a manual font with FontManager");
		return *font->castType<Font>();
	}

	const MyGUI::GlyphInfo& glyph(const Font& _font, MyGUI::Char _code)
	{
		const auto* result = _font.getGlyphInfo(_code);
		require(result != nullptr && result->codePoint == _code, "Manual glyph must exist without substitution");
		return *result;
	}

	void testXmlMetrics()
	{
		unittest::FontTestContext context;
		auto& texture = createAtlas(context);
		auto& font = loadFont(R"(
			<Code index="65" coord="16 8 24 16" size="12 8" bearing="-2 3" advance="13.5"/>
			<Code index="66" coord="48 24 10 14"/>
			<Code index="67" coord="64 24 16 20" size="8 10"/>
		)");
		require(
			font.getTextureFont() == &texture && context.renderer.created == 1,
			"Source must reuse an existing atlas texture");
		require(
			texture.shader == "ManualFontShader" && font.getDefaultHeight() == 20,
			"XML must apply shader and default font height");
		const auto& a = glyph(font, 'A');
		require(
			a.uvRect == MyGUI::FloatRect(16.0f / 128, 8.0f / 64, 40.0f / 128, 24.0f / 64),
			"XML atlas coordinates must become normalized UVs using both texture dimensions");
		require(
			a.width == 12 && a.height == 8 && a.advance == 13.5f && a.bearingX == -2 && a.bearingY == 3,
			"Logical glyph size, advance and bearings must remain independent of atlas dimensions");
		const auto& b = glyph(font, 'B');
		require(
			b.width == 10 && b.height == 14 && b.advance == 10 && b.bearingX == 0 && b.bearingY == 0,
			"Omitted size and advance must default to atlas rectangle size, with zero bearings");
		const auto& c = glyph(font, 'C');
		require(
			c.width == 8 && c.height == 10 && c.advance == 8,
			"Omitted advance must use the explicit logical width when size is supplied");
	}

	void testLegacyAdvance()
	{
		unittest::FontTestContext context;
		createAtlas(context);
		const char* codes = R"(
			<Code index="65" coord="0 0 8 12" bearing="3 1" advance="10"/>
			<Code index="66" coord="16 0 8 12" bearing="-2 1"/>
		)";
		auto& legacy = loadFont(codes, MyGUI::Version(1, 1));
		auto& modern = loadFont(codes);
		require(
			glyph(legacy, 'A').advance == 13 && glyph(legacy, 'B').advance == 6,
			"Version 1.1 must add horizontal bearing to explicit and default advances");
		require(
			glyph(modern, 'A').advance == 10 && glyph(modern, 'B').advance == 8,
			"Version 1.2 must keep advances independent of horizontal bearing");
		require(
			glyph(legacy, 'A').uvRect == glyph(modern, 'A').uvRect &&
				glyph(legacy, 'A').bearingX == glyph(modern, 'A').bearingX,
			"Legacy advance conversion must preserve UV coordinates and bearings");
	}

	void testSpecialGlyphsAndFallback()
	{
		unittest::FontTestContext context;
		createAtlas(context);
		auto& font = loadFont(R"(
			<Code index="cursor" coord="0 0 2 20" advance="0"/>
			<Code index="selected" coord="4 0 1 20" advance="0"/>
			<Code index="selected_back" coord="8 0 1 20" advance="0"/>
			<Code index="substitute" coord="16 0 8 12" advance="9"/>
			<Code index="128512" coord="32 0 16 16"/>
		)");
		for (auto code :
			 {MyGUI::FontCodeType::Cursor, MyGUI::FontCodeType::Selected, MyGUI::FontCodeType::SelectedBack})
			require(glyph(font, code).advance == 0, "Named editing glyphs must map to MyGUI special code points");
		const auto* fallback = &glyph(font, MyGUI::FontCodeType::NotDefined);
		require(
			font.getGlyphInfo('Z') == fallback && font.getGlyphInfo(0x10FFFF) == fallback,
			"Missing BMP and supplementary characters must use the declared substitute");
		require(glyph(font, 0x1F600).advance == 16, "Manual font XML must retain supplementary code points");
		auto& noFallback = loadFont("<Code index=\"65\" coord=\"0 0 8 12\"/>");
		require(noFallback.getGlyphInfo('Z') == nullptr, "Missing glyph without a substitute must return nullptr");
	}

	void testKerning()
	{
		unittest::FontTestContext context;
		createAtlas(context);
		auto& font = loadFont(R"(
			<Code index="65" coord="0 0 8 12">
				<Kerning right="86" offset="-2.5"/>
				<Kerning right="128512" offset="1.25"/>
			</Code>
			<Code index="86" coord="16 0 8 12"/>
			<Code index="128512" coord="32 0 16 16"/>
		)");
		require(
			font.getKerning('A', 'V') == -2.5f && font.getKerning('A', 0x1F600) == 1.25f,
			"XML must preserve signed fractional kerning and supplementary pair members");
		require(
			font.getKerning('V', 'A') == 0 && font.getKerning('A', 'Z') == 0,
			"Kerning must be directional and absent pairs must return zero");
		font.addKerningInfo('A', 'V', -1);
		require(
			font.getKerning('A', 'V') == -1 && font.getKerning('A', 0x1F600) == 1.25f,
			"Programmatic kerning must replace the selected pair without changing others");
	}

	void testProgrammaticFontAndTextureOwnership()
	{
		unittest::FontTestContext context;
		auto& first = createAtlas(context);
		auto& second = createAtlas(context, "SecondAtlas");
		{
			Font font;
			font.setSource("ManualAtlas");
			font.setDefaultHeight(24);
			font.setShader("FirstShader");
			font.addGlyphInfo('A', MyGUI::GlyphInfo('A', 8, 12, 9, -1, 2, {0, 0, 0.5f, 0.5f}));
			font.addGlyphInfo(
				MyGUI::FontCodeType::NotDefined,
				MyGUI::GlyphInfo(MyGUI::FontCodeType::NotDefined, 6, 12, 7));
			require(
				font.getTextureFont() == &first && first.shader == "FirstShader" && font.getDefaultHeight() == 24,
				"Programmatic source, shader and height must be applied");
			require(
				glyph(font, 'A').advance == 9 && glyph(font, 'A').bearingX == -1,
				"Programmatic glyph metrics must be available through IFont");
			require(
				font.getGlyphInfo('Z') == &glyph(font, MyGUI::FontCodeType::NotDefined),
				"Programmatically added NotDefined glyph must become the substitute");
			font.setTexture(&second);
			font.setShader("SecondShader");
			require(
				font.getTextureFont() == &second && second.shader == "SecondShader" && first.shader == "FirstShader",
				"Changing texture must direct subsequent shader changes to the new atlas");
			font.setSource("ManualAtlas");
			require(font.getTextureFont() == &first, "Changing source must replace an explicitly assigned texture");
		}
		require(
			context.renderer.destroyed == 0 && context.renderer.textures.size() == 2,
			"Manual fonts must not destroy externally owned textures when replaced or destroyed");
		auto& shared = loadFont("<Code index=\"65\" coord=\"0 0 8 12\"/>");
		auto& survivor = loadFont("<Code index=\"86\" coord=\"16 0 8 12\"/>");
		const auto name = shared.getResourceName();
		require(MyGUI::ResourceManager::getInstance().removeByName(name), "Manual resource must be removable");
		require(
			survivor.getTextureFont() == &first && context.renderer.destroyed == 0,
			"Removing one resource must preserve the shared atlas used by another font");
	}

	void testTextLayout()
	{
		unittest::FontTestContext context;
		createAtlas(context);
		auto& font = loadFont(R"(
			<Code index="65" coord="0 0 16 24" size="8 12" bearing="-2 3" advance="10">
				<Kerning right="86" offset="-2.5"/>
			</Code>
			<Code index="86" coord="16 0 18 24" size="9 12" advance="11"/>
			<Code index="9" coord="0 0 0 0" advance="24"/>
			<Code index="substitute" coord="40 0 8 12" advance="7"/>
		)");
		auto* text = unittest::createFontTextBox(context.gui, font);
		text->setCaption("AV");
		require(
			text->getTextSize() == MyGUI::IntSize(19, 20),
			"TextBox must use logical advances and fractional kerning, rounding once per line");
		text->setCaption("A\tV\nZ");
		require(
			text->getTextSize() == MyGUI::IntSize(45, 40),
			"Manual tab and substitute metrics must participate in multiline text layout");
		text->setCaption("Z");
		require(text->getTextSize() == MyGUI::IntSize(7, 20), "TextBox must measure the manual substitute glyph");
		text->setFontHeight(40);
		text->setCaption("AV");
		require(
			text->getTextSize() == MyGUI::IntSize(37, 40),
			"Height scaling must precede rounding of manual metrics");
	}

}

int main()
{
	return unittest::runTests({
		{"XML atlas coordinates and logical metrics", testXmlMetrics},
		{"legacy and modern advance formats", testLegacyAdvance},
		{"special names, Unicode and missing glyphs", testSpecialGlyphsAndFallback},
		{"XML and programmatic kerning", testKerning},
		{"programmatic fonts and shared texture ownership", testProgrammaticFontAndTextureOwnership},
		{"manual fonts in TextBox layout", testTextLayout},
	});
}
