#include "BehaviourTestSupport.h"
#include "FixedFont.h"
#include "MyGUI_ResourceManualFont.h"
#include <array>

namespace
{

	using unittest::require;

	MyGUI::EditBox* createText(MyGUI::Gui& _gui)
	{
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
		unittest::registerFixedFont();
		unittest::loadResources("UnitTest_Text/TestSkin.xml");
		auto* text = _gui.createWidget<MyGUI::EditBox>(
			"BehaviourTextBox",
			MyGUI::IntCoord(0, 0, 200, 200),
			MyGUI::Align::Default,
			"Main");
		text->setEditMultiLine(true);
		return text;
	}

	void update(MyGUI::EditBox* _view, const char* _text, int _width = -1)
	{
		_view->setSize(_width < 0 ? 200 : _width, 200);
		_view->setEditWordWrap(_width >= 0);
		_view->setCaption(MyGUI::UString(_text));
	}

	void testUnicodeRoundTrip()
	{
		const std::string bytes = "A\xD0\x91\xE4\xB8\xAD\xF0\x9F\x98\x80\xF4\x8F\xBF\xBF";
		const MyGUI::UString::utf32string expected = {'A', 0x411, 0x4E2D, 0x1F600, 0x10FFFF};
		MyGUI::UString text(bytes);
		require(text.asUTF32() == expected, "UTF-8 decoding must preserve BMP and non-BMP characters");
		require(MyGUI::UString(expected).asUTF8() == bytes, "UTF-32 to UTF-8 must round-trip exactly");
		text.append(MyGUI::UString("!"));
		require(text.asUTF8() == bytes + "!", "UTF-8 conversion must reflect mutations after a cached conversion");
		text.assign(std::string_view("replacement"));
		require(text.asUTF32() == MyGUI::UString("replacement").asUTF32(), "UTF-32 conversion must reflect assignment");
		const std::string withNull("a\0b", 3);
		require(MyGUI::UString(withNull).asUTF8() == withNull, "Length-delimited strings must preserve embedded nulls");
		require(MyGUI::UString("").asUTF32().empty(), "Empty strings must convert to empty strings");
	}

	void testMalformedUtf8()
	{
		// The API does not mandate rejection versus recovery. Both must stay within the
		// supplied byte range and produce valid Unicode if conversion succeeds.
		for (const auto& bytes : std::array<std::string, 4>{
				 "\xC2",
				 "\xE2\x82",
				 "\xF0\x9F\x98",
				 "\xE2"
				 "A\xAC"})
		{
			try
			{
				const std::vector<char> bounded(bytes.begin(), bytes.end());
				const MyGUI::UString text(bounded.data(), bounded.size());
				const auto decoded = text.asUTF32();
				require(
					decoded.size() <= bytes.size(),
					"Malformed input must not consume bytes beyond the supplied range");
				for (const auto character : decoded)
					require(
						character <= 0x10FFFF && !(character >= 0xD800 && character <= 0xDFFF),
						"Recovered text must contain valid Unicode scalar values");
				require(
					MyGUI::UString(text.asUTF8()).asUTF32() == decoded,
					"Recovered text must have a valid UTF-8 round trip");
				const MyGUI::UString followed(bytes + "ok");
				require(
					followed.asUTF8().substr(followed.asUTF8().size() - 2) == "ok",
					"Recovery must preserve valid text following malformed input");
			}
			catch (const MyGUI::UString::invalid_data&)
			{
				// Rejecting malformed input is also an acceptable contract.
			}
		}
	}

	void testLinesAndTags()
	{
		unittest::TestContext context;
		auto* view = createText(context.getGui());
		update(view, "ab");
		require(
			view->getTextSize() == MyGUI::IntSize(20, 20),
			"Two fixed-width characters must occupy 20 by 20 pixels");
		update(view, "#FF0000ab");
		require(
			view->getTextLength() == 2 && view->getTextSize() == MyGUI::IntSize(20, 20),
			"Colour tags must not consume text width or cursor positions");
		update(view, "a##b");
		require(
			view->getTextLength() == 3 && view->getTextSize().width == 30,
			"An escaped hash must occupy one character");
		for (const char* text : {"ab\nc", "ab\r\nc", "ab\rc"})
		{
			update(view, text);
			require(view->getTextSize() == MyGUI::IntSize(20, 40), "LF, CRLF, and CR must each create one line break");
			require(view->getTextLength() == 4, "CRLF must count as one logical newline");
		}
		update(view, "a\n");
		require(view->getTextSize().height == 40, "A trailing newline must leave an empty final line");
		update(view, "");
		require(
			view->getTextLength() == 0 && view->getTextSize().width == 0,
			"Clearing text must discard previous layout data");
	}

	void testWrapping()
	{
		unittest::TestContext context;
		auto* view = createText(context.getGui());
		update(view, "abcd", 40);
		require(
			view->getTextSize() == MyGUI::IntSize(40, 20),
			"Text exactly fitting the width must remain on one line");
		update(view, "abcd", 25);
		require(
			view->getTextSize() == MyGUI::IntSize(20, 40),
			"A long word must wrap without dropping or duplicating characters");
		require(view->getTextLength() == 4, "Soft wrapping must preserve logical text length");
		update(view, "ab cd", 35);
		require(view->getTextSize().height == 40, "Words must wrap at an available space");
		require(view->getTextLength() == 5, "Wrapping at a space must preserve logical text length");
		update(view, "abcd", 100);
		require(
			view->getTextSize() == MyGUI::IntSize(40, 20),
			"Growing the available width must remove obsolete line breaks");
	}

	void testCursorHitTesting()
	{
		unittest::TestContext context;
		auto* view = createText(context.getGui());
		update(view, "abc\nde");
		const MyGUI::IntPoint expected[] = {{0, 0}, {10, 0}, {20, 0}, {30, 0}, {0, 20}, {10, 20}, {20, 20}};
		for (size_t index = 0; index < 7; ++index)
		{
			require(
				view->getClientWidget()->getSubWidgetText()->getCursorCoord(index).point() == expected[index],
				"Cursor coordinates must follow fixed metrics and explicit newlines");
			require(
				view->getClientWidget()->getSubWidgetText()->getCursorPosition(expected[index]) == index,
				"Hit testing a cursor location must recover its logical index");
		}
		require(
			view->getClientWidget()->getSubWidgetText()->getCursorPosition(MyGUI::IntPoint(4, 0)) == 0,
			"Left half of a glyph must hit its leading cursor boundary");
		require(
			view->getClientWidget()->getSubWidgetText()->getCursorPosition(MyGUI::IntPoint(6, 0)) == 1,
			"Right half of a glyph must hit its trailing cursor boundary");
		require(
			view->getClientWidget()->getSubWidgetText()->getCursorPosition(MyGUI::IntPoint(-20, 0)) == 0,
			"Hits left of a line must clamp to its start");
		require(
			view->getClientWidget()->getSubWidgetText()->getCursorPosition(MyGUI::IntPoint(200, 20)) == 6,
			"Hits right of the last line must clamp to its end");
	}

	void testManualFontMetrics()
	{
		unittest::TestContext context;
		auto* view = createText(context.getGui());
		auto* fontResource = new MyGUI::ResourceManualFont();
		fontResource->setResourceName("KerningFont");
		MyGUI::ResourceManager::getInstance().addResource(fontResource);
		auto& font = *fontResource;
		font.setDefaultHeight(20);
		font.addGlyphInfo('A', MyGUI::GlyphInfo('A', 10, 20, 10));
		font.addGlyphInfo('V', MyGUI::GlyphInfo('V', 10, 20, 10));
		font.addKerningInfo('A', 'V', -2);
		require(font.getGlyphInfo('x') == nullptr, "A manual font without a substitute must report absent glyphs");
		require(font.getKerning('V', 'A') == 0, "Unspecified kerning must be zero");
		view->setFontName("KerningFont");
		update(view, "AV");
		require(view->getTextSize().width == 18, "Text width must include pair kerning");
		update(view, "A\nV");
		require(view->getTextSize() == MyGUI::IntSize(10, 40), "Kerning must not cross a newline");
	}

}

int main()
{
	return unittest::runTests({
		{"Unicode round trips and mutation", testUnicodeRoundTrip},
		{"Malformed UTF-8", testMalformedUtf8},
		{"Lines and colour tags", testLinesAndTags},
		{"Wrapping boundaries", testWrapping},
		{"Cursor hit testing", testCursorHitTesting},
		{"Manual font metrics", testManualFontMetrics},
	});
}
