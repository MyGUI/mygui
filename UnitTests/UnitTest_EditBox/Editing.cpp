#include "Editing.h"
#include "BehaviourTestSupport.h"
#include "FixedFont.h"
#include <limits>

namespace
{

	using unittest::require;

	struct Fixture
	{
		Fixture()
		{
			unittest::createInputLayer();
			unittest::registerFixedFont();
			unittest::loadResources("UnitTest_EditBox/TestSkin.xml");
			edit = context.getGui().createWidget<MyGUI::EditBox>(
				"BehaviourEditBox",
				MyGUI::IntCoord(0, 0, 200, 80),
				MyGUI::Align::Default,
				"Main");
			MyGUI::InputManager::getInstance().setKeyFocusWidget(edit);
		}
		unittest::TestContext context;
		MyGUI::EditBox* edit;
	};

	// Read the public caption's markup independently of TextIterator's editing machinery.
	std::vector<unsigned int> colours(const MyGUI::UString& _caption)
	{
		std::vector<unsigned int> result;
		unsigned int colour = 0xFFFFFF;
		const auto& text = _caption.asUTF32();
		for (size_t i = 0; i < text.size(); ++i)
		{
			if (text[i] == U'#')
			{
				require(i + 1 < text.size(), "Colour edits must not leave a dangling hash");
				if (text[i + 1] != U'#')
				{
					require(i + 6 < text.size(), "Colour edits must emit complete colour tags");
					colour = 0;
					for (int digit = 0; digit < 6; ++digit)
					{
						const auto c = text[++i];
						require((c >= U'0' && c <= U'9') || (c >= U'A' && c <= U'F'), "Expected hexadecimal tag");
						colour = colour * 16 + (c <= U'9' ? c - U'0' : c - U'A' + 10);
					}
					continue;
				}
				++i;
			}
			result.push_back(colour);
		}
		return result;
	}

	void modified(MyGUI::KeyCode _modifier, MyGUI::KeyCode _key)
	{
		auto& input = MyGUI::InputManager::getInstance();
		input.injectKeyPress(_modifier);
		unittest::keyStroke(_key);
		input.injectKeyRelease(_modifier);
	}

}

void testEditColours()
{
	Fixture f;
	auto* edit = f.edit;
	const MyGUI::UString plain(std::u32string(U"A\U0001F600B#CD"));
	edit->setCaption(MyGUI::UString(std::u32string(U"#FF0000A\U0001F600#00FF00B##C#0000FFD")));
	edit->setTextCursor(5);
	edit->setTextIntervalColour(1, 3, MyGUI::Colour(1, 1, 0));
	require(
		edit->getOnlyText() == plain && edit->getTextLength() == 6 && edit->getTextCursor() == 5,
		"Recolouring must preserve Unicode, literal hashes, logical length and cursor");
	require(
		colours(edit->getCaption()) ==
			std::vector<unsigned int>({0xFF0000, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0x00FF00, 0x0000FF}),
		"A range must replace interior tags and restore the following character's original colour");
	edit->setTextSelection(5, 2);
	edit->setTextSelectionColour(MyGUI::Colour::Blue);
	require(
		colours(edit->getCaption()) ==
				std::vector<unsigned int>({0xFF0000, 0xFFFF00, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF}) &&
			edit->getTextSelectionStart() == 2 && edit->getTextSelectionEnd() == 5 && edit->getTextCursor() == 2,
		"Recolouring a reverse selection must use ordered bounds and preserve its active end");
	edit->setTextCursor(0);
	const auto caption = edit->getCaption();
	edit->setTextSelectionColour(MyGUI::Colour::Green);
	require(edit->getCaption() == caption, "Recolouring with no selection must leave markup unchanged");
	edit->setTextIntervalColour(0, 6, MyGUI::Colour::Green);
	require(
		colours(edit->getCaption()) == std::vector<unsigned int>(6, 0x00FF00) && edit->getOnlyText() == plain,
		"Whole-text recolouring must replace every previous colour without changing text");
}

void testEditColourBoundaries()
{
	Fixture f;
	auto* edit = f.edit;
	edit->setCaption("#FF0000abcd");
	const auto caption = edit->getCaption();
	edit->setTextIntervalColour(2, 0, MyGUI::Colour::Green);
	require(edit->getCaption() == caption, "An empty colour range must not recolour its suffix");
	edit->setTextIntervalColour(4, 10, MyGUI::Colour::Green);
	edit->setTextIntervalColour(std::numeric_limits<size_t>::max(), 2, MyGUI::Colour::Green);
	require(edit->getCaption() == caption, "Colour ranges starting at or beyond the end must be harmless");
	edit->setTextIntervalColour(2, std::numeric_limits<size_t>::max(), MyGUI::Colour::Green);
	require(
		edit->getOnlyText() == "abcd" &&
			colours(edit->getCaption()) == std::vector<unsigned int>({0xFF0000, 0xFF0000, 0x00FF00, 0x00FF00}),
		"Oversized colour counts must clamp to the suffix without integer overflow");
	edit->setOnlyText("");
	edit->setTextIntervalColour(0, 1, MyGUI::Colour::Red);
	require(edit->getCaption().empty(), "Recolouring an empty field must not insert formatting tags");
}

void testEditKeyboardNavigation()
{
	Fixture f;
	auto* edit = f.edit;
	auto& input = MyGUI::InputManager::getInstance();
	edit->setEditMultiLine(true);
	edit->setOnlyText("abcd\nabcd\nabcd\nabcd\nabcd\nabcd\nabcd\nabcd\nabcd\nabcd");
	edit->setTextCursor(2);
	unittest::keyStroke(MyGUI::KeyCode::PageDown);
	require(edit->getTextCursor() == 22, "PageDown must advance one 80-pixel viewport while retaining the column");
	unittest::keyStroke(MyGUI::KeyCode::PageUp);
	require(edit->getTextCursor() == 2, "PageUp must restore the previous column and line");
	edit->setSize(200, 10);
	unittest::keyStroke(MyGUI::KeyCode::PageDown);
	require(edit->getTextCursor() == 7, "A viewport shorter than one line must still page by one line");
	unittest::keyStroke(MyGUI::KeyCode::PageUp);
	require(edit->getTextCursor() == 2, "PageUp in a short viewport must move one line");
	edit->setSize(200, 80);
	unittest::keyStroke(MyGUI::KeyCode::PageUp);
	require(edit->getTextCursor() == 0, "Paging above the first line must clamp to the document beginning");
	edit->setTextCursor(47);
	unittest::keyStroke(MyGUI::KeyCode::PageDown);
	require(edit->getTextCursor() == 49, "Paging below the last line must clamp to the document end");
	edit->setTextCursor(2);
	input.injectKeyPress(MyGUI::KeyCode::LeftShift);
	unittest::shortcut(MyGUI::KeyCode::End);
	input.injectKeyRelease(MyGUI::KeyCode::LeftShift);
	require(
		edit->getTextCursor() == 49 && edit->getTextSelectionStart() == 2 && edit->getTextSelectionEnd() == 49,
		"Shift plus document-end shortcut must select through the last character");
	unittest::shortcut(MyGUI::KeyCode::End);
	require(!edit->isTextSelection(), "Document-end navigation at the boundary must clear selection without Shift");
	input.injectKeyPress(MyGUI::KeyCode::LeftShift);
	unittest::shortcut(MyGUI::KeyCode::Home);
	input.injectKeyRelease(MyGUI::KeyCode::LeftShift);
	require(
		edit->getTextCursor() == 0 && edit->getTextSelectionLength() == 49,
		"Document-start shortcut must support reverse selection");
	unittest::shortcut(MyGUI::KeyCode::Home);
	require(!edit->isTextSelection(), "Document-start navigation at the boundary must clear selection");
	for (const auto key : {MyGUI::KeyCode::ArrowLeft, MyGUI::KeyCode::Home})
	{
		edit->setTextSelection(2, 0);
		unittest::keyStroke(key);
		require(
			edit->getTextCursor() == 0 && !edit->isTextSelection(),
			"Navigation at the beginning must clear selection");
	}
	for (const auto key : {MyGUI::KeyCode::ArrowRight, MyGUI::KeyCode::End})
	{
		edit->setTextSelection(47, 49);
		unittest::keyStroke(key);
		require(edit->getTextCursor() == 49 && !edit->isTextSelection(), "Navigation at the end must clear selection");
	}
#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
	edit->setTextCursor(7);
	unittest::shortcut(MyGUI::KeyCode::ArrowLeft);
	require(edit->getTextCursor() == 5, "Command+Left must move to the current line beginning");
	modified(MyGUI::KeyCode::LeftControl, MyGUI::KeyCode::ArrowRight);
	require(edit->getTextCursor() == 9, "Control+Right must move to the current line end on macOS");
	unittest::shortcut(MyGUI::KeyCode::ArrowUp);
	require(edit->getTextCursor() == 0, "Command+Up must move to the document beginning");
	unittest::shortcut(MyGUI::KeyCode::ArrowDown);
	require(edit->getTextCursor() == 49, "Command+Down must move to the document end");
#endif
	edit->setEditMultiLine(false);
	edit->setOnlyText("one two three");
	edit->setEditPassword(true);
	edit->setTextCursor(5);
#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
	const auto wordModifier = MyGUI::KeyCode::LeftAlt;
#else
	const auto wordModifier = MyGUI::KeyCode::LeftControl;
#endif
	modified(wordModifier, MyGUI::KeyCode::ArrowRight);
	require(
		edit->getTextCursor() == 13,
		"Word navigation in a password must skip to the end without exposing word boundaries");
	modified(wordModifier, MyGUI::KeyCode::ArrowLeft);
	require(edit->getTextCursor() == 0, "Reverse word navigation in a password must skip to the beginning");
}

void testEditKeyboardCommands()
{
	int accepted = 0;
	Fixture f;
	auto* edit = f.edit;
	edit->eventEditSelectAccept += MyGUI::newDelegate([&](MyGUI::EditBox*) { ++accepted; }, 1);
	edit->setOnlyText(MyGUI::UString(std::u32string(U"A\U0001F600#")));
	unittest::shortcut(MyGUI::KeyCode::A);
	require(
		edit->getTextSelectionStart() == 0 && edit->getTextSelectionLength() == 3,
		"Select-all must count logical Unicode characters");
	unittest::keyStroke(MyGUI::KeyCode::X, 'x');
	require(edit->getOnlyText() == "x", "Typing after select-all must replace all text");
	unittest::shortcut(MyGUI::KeyCode::Z);
	require(edit->getOnlyText().asUTF32() == U"A\U0001F600#", "One undo must restore a select-all replacement");
	edit->setOnlyText("ab");
	edit->setTextCursor(1);
	unittest::keyStroke(MyGUI::KeyCode::Tab, '\t');
	require(edit->getOnlyText() == "ab", "Tab must not insert text when tab printing is disabled");
	edit->setTabPrinting(true);
	unittest::keyStroke(MyGUI::KeyCode::Tab, '\t');
	require(
		edit->getOnlyText() == "a\tb" && edit->getTextCursor() == 2,
		"Enabled tab printing must insert one logical character");
	unittest::shortcut(MyGUI::KeyCode::Z);
	require(edit->getOnlyText() == "ab", "Tab insertion must be undoable");
	edit->setEditMultiLine(true);
	unittest::shortcut(MyGUI::KeyCode::NumpadEnter);
	require(
		accepted == 1 && edit->getOnlyText() == "ab",
		"Modified Enter must accept multiline text without inserting a newline");
	edit->setEditReadOnly(true);
	unittest::keyStroke(MyGUI::KeyCode::Return);
	require(
		accepted == 1 && edit->getOnlyText() == "ab",
		"Plain Enter in read-only multiline mode must not modify or accept text");
	unittest::keyStroke(MyGUI::KeyCode::Escape);
	require(MyGUI::InputManager::getInstance().getKeyFocusWidget() == nullptr, "Escape must release keyboard focus");
}
