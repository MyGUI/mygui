#include "TestSupport.h"
#include "TestRunner.h"
#include "FixedFont.h"
#include "MyGUI_ResourceManualFont.h"
#include "MyGUI_TextIterator.h"
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
		require(text.asUTF8() == bytes + "!", "UTF-8 conversion must reflect mutations after conversion");
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

	void testMalformedWideStrings()
	{
		const auto check = [](const std::wstring& input, const std::u32string& expected)
		{
			const MyGUI::UString text(input);
			require(text.asUTF32() == expected, "Wide-string recovery must preserve valid neighbouring text");
			require(MyGUI::UString(text.asUTF8()) == text, "Recovered wide input must produce valid UTF-8");
			require(MyGUI::UString(text.asWStr()) == text, "Recovered wide input must produce valid native text");
		};
		const wchar_t high = 0xD800;
		const wchar_t low = 0xDC00;
		check({}, U"");
		check({high}, U"\uFFFD");
		check({low}, U"\uFFFD");
		check({L'A', high, L'B', low, L'C', high}, U"A\uFFFDB\uFFFDC\uFFFD");
		check({low, high}, U"\uFFFD\uFFFD");
		check({high, L'\0', low}, std::u32string(U"\uFFFD\0\uFFFD", 3));
		check(L"A\U00010000\U0010FFFFB", U"A\U00010000\U0010FFFFB");
		require(
			MyGUI::UString(std::wstring{high, L'B'}.c_str()).asUTF32() == U"\uFFFDB",
			"Null-terminated wide input must use the same replacement policy");
		if constexpr (sizeof(wchar_t) == 2)
		{
			check({high, high, low}, U"\uFFFD\U00010000");
			check({high, low, low}, U"\U00010000\uFFFD");
		}
		else
		{
			check({high, low}, U"\uFFFD\uFFFD");
			check({static_cast<wchar_t>(0x110000), L'B'}, U"\uFFFDB");
			check({static_cast<wchar_t>(-1)}, U"\uFFFD");
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

	void testStringAdapter()
	{
		MyGUI::UString text("A\xF0\x9F\x98\x80#");
		require(text.size() == 3 && text.find('#') == 2, "UString offsets must count Unicode code points");
		const auto& utf8 = text.asUTF8();
		const auto& utf32 = text.asUTF32();
		const auto wide = text.asWStr();
		require(utf8 == "A\xF0\x9F\x98\x80#", "Other conversions must preserve the UTF-8 result");
		require(utf32 == U"A\U0001F600#", "Wide conversion must preserve the UTF-32 result");
		require(MyGUI::UString(wide) == text, "Native wide strings must round-trip through the adapter");
		require(&text[0] == utf32.data(), "UTF-32 access must refer directly to primary storage");
		require(
			text.substr(1, 1).asUTF32() == U"\U0001F600",
			"A substring must preserve a complete supplementary character");
		*text.begin() = U'B';
		require(text.asUTF8() == "B\xF0\x9F\x98\x80#", "Conversion must reflect edits through standard iterators");
		MyGUI::UString copied(text);
		text[1] = U'\U0001D800';
		require(copied.asUTF32() == U"B\U0001F600#", "Copying text must retain independent primary storage");
		const auto& copiedUtf8 = copied.asUTF8();
		const auto copiedWide = copied.asWStr();
		copied = text;
		require(copiedUtf8.empty(), "Copy assignment must leave the UTF-8 buffer empty");
		require(copiedWide == L"B\U0001F600#", "Copy assignment must preserve previously returned wide strings");
		require(copied.asWStr() == L"B\U0001D800#", "Wide conversion must reflect the assigned text");
		require(wide == L"A\U0001F600#", "Later conversions and edits must preserve earlier wide strings");
		const auto temporaryWide = MyGUI::UString("\xC3\xA9\xF0\x9F\x98\x80").asWStr();
		require(temporaryWide == L"\u00E9\U0001F600", "Wide strings must outlive the source UString");
		require(MyGUI::UString().asWStr().empty(), "Empty text must produce an empty owning wide string");
		text[1] = U'\U0001F600';
		text.assign(text.asUTF8());
		require(text[0] == U'B', "Assignment must accept the object's own conversion buffer");
		const auto moved = std::move(text);
		require(moved.asUTF32() == U"B\U0001F600#", "Moving the wrapper must preserve its text");
		text.assign(std::string_view());
		require(text.empty(), "A moved-from wrapper must accept an empty string_view");
		text = "x";
		text[0] = static_cast<MyGUI::UString::code_point>(0xD800);
		bool translated = false;
		try
		{
			text.asUTF8();
		}
		catch (const MyGUI::UString::invalid_data&)
		{
			translated = true;
		}
		require(translated, "Conversion errors must use the MyGUI exception type");

#if defined(__cpp_char8_t)
		const MyGUI::UString literal = u8"A\U0001F600";
		require(literal == "A\xF0\x9F\x98\x80", "UTF-8 literals must accept char8_t input");
		const std::u8string bytes(u8"A\U0001F600\0B", 7);
		const std::string expected("A\xF0\x9F\x98\x80\0B", 7);
		const MyGUI::UString owned = bytes;
		const MyGUI::UString viewed{std::u8string_view(bytes)};
		const MyGUI::UString bounded(bytes.data(), bytes.size());
		require(
			owned.asUTF8() == expected && viewed == owned && bounded == owned,
			"char8_t string, view and bounded input must preserve embedded nulls");
		text.assign(std::u8string_view(bytes));
		require(text == owned, "Assignment must accept UTF-8 char8_t views");
		text.assign(std::u8string_view());
		require(text.empty(), "An empty char8_t view must clear the text");
		require(
			MyGUI::UString(std::u8string_view()).empty() &&
				MyGUI::UString(static_cast<const char8_t*>(nullptr), 0).empty(),
			"Empty char8_t input must accept a null data pointer");
#endif
	}

	void testTextIteratorCodePointEdits()
	{
		const MyGUI::UString source(MyGUI::UString::utf32string(U"\U0001F600#\r\n\u0085#\U0001F600"));
		const auto escaped = MyGUI::TextIterator::toTagsString(source);
		require(
			escaped.asUTF32() == U"\U0001F600##\r\n\u0085##\U0001F600",
			"Hash escaping must preserve neighbouring supplementary characters and newlines");
		MyGUI::UString flattened = source;
		MyGUI::TextIterator iterator{MyGUI::UString()};
		iterator.clearNewLine(flattened);
		require(
			flattened.asUTF32() == U"\U0001F600#   #\U0001F600",
			"Newline replacement must preserve neighbouring supplementary characters and hashes");
		iterator.setText(escaped, true);
		require(
			iterator.getText().asUTF32() == U"\U0001F600##\n\u0085##\U0001F600",
			"CRLF normalization must preserve supplementary characters and escaped hashes");
		iterator.setText(iterator.getText(), true);
		require(
			iterator.getText().asUTF32() == U"\U0001F600##\n\u0085##\U0001F600",
			"Replacing iterator text with its own reference must preserve the text");
		const MyGUI::UString tag(std::u32string(U"#\U0001D80012345"));
		MyGUI::TextIterator tagged(tag + "text");
		MyGUI::UString extracted;
		require(
			tagged.getTagColour(extracted) && extracted == tag,
			"Extracting an unvalidated colour tag must preserve its full code points");
	}

	void testConsecutiveColourTags()
	{
		MyGUI::TextIterator iterator(MyGUI::UString("#112233#AABBCC##text"));
		MyGUI::UString colour;
		require(
			iterator.getTagColour(colour) && colour == "#AABBCC",
			"Only the final consecutive colour tag must be returned");
		iterator.clearTagColour();
		require(iterator.getText() == "##text", "Clearing tags must preserve an escaped hash");
		require(!iterator.getTagColour(colour), "An escaped hash must not be treated as a colour tag");
		MyGUI::TextIterator incomplete(MyGUI::UString("#112233#AA"));
		require(
			incomplete.getTagColour(colour) && colour == "#112233",
			"An incomplete trailing tag must not replace the last complete tag");
		incomplete.clearTagColour();
		require(
			incomplete.getText() == "#AA",
			"Clearing complete tags must leave an incomplete trailing tag untouched");
	}

	void testTextIteratorTruncatedColourTags()
	{
		for (const char* text : {"a#0", "a#00", "a#000", "a#0000", "a#00000"})
		{
			MyGUI::TextIterator iterator{MyGUI::UString()};
			iterator.setText(MyGUI::UString(text), true);
			iterator.cutMaxLengthFromBeginning(0);
			require(iterator.getSize() == 0, "Left-side truncation must remove all logical characters");
			require(
				MyGUI::TextIterator::getOnlyText(iterator.getText()).asUTF32().empty(),
				"Truncated incomplete colour tags must not leave visible text");
		}
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
		{"Consecutive colour tags", testConsecutiveColourTags},
		{"Unicode round trips and mutation", testUnicodeRoundTrip},
		{"Malformed UTF-8", testMalformedUtf8},
		{"Malformed native wide strings", testMalformedWideStrings},
		{"Lines and colour tags", testLinesAndTags},
		{"UString adapter compatibility", testStringAdapter},
		{"TextIterator code-point edits", testTextIteratorCodePointEdits},
		{"Truncated colour tags in TextIterator", testTextIteratorTruncatedColourTags},
		{"Wrapping boundaries", testWrapping},
		{"Cursor hit testing", testCursorHitTesting},
		{"Manual font metrics", testManualFontMetrics},
	});
}
