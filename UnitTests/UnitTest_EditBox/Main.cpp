#include "BehaviourTestSupport.h"
#include "TestRunner.h"
#include "FixedFont.h"

#include "Interaction.h"
#include "Editing.h"

namespace
{

	using unittest::require;

	MyGUI::EditBox* createEdit(MyGUI::Gui& _gui)
	{
		unittest::createInputLayer();
		unittest::registerFixedFont();
		unittest::loadResources("UnitTest_EditBox/TestSkin.xml");
		auto* edit = _gui.createWidget<MyGUI::EditBox>(
			"BehaviourEditBox",
			MyGUI::IntCoord(0, 0, 200, 80),
			MyGUI::Align::Default,
			"Main");
		MyGUI::InputManager::getInstance().setKeyFocusWidget(edit);
		return edit;
	}

	void testIntervals()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		edit->setOnlyText("bc");
		edit->insertText("a", 0);
		edit->addText("d");
		require(edit->getOnlyText() == "abcd", "Insertion at both boundaries must preserve existing text");
		edit->eraseText(0);
		edit->eraseText(2);
		require(edit->getOnlyText() == "bc", "Deletion at both boundaries must remove only the requested character");
		edit->setTextSelection(0, 2);
		require(
			edit->getTextSelectionLength() == 2 && MyGUI::TextIterator::getOnlyText(edit->getTextSelection()) == "bc",
			"Selection must use an exclusive end");
		edit->setTextSelection(2, 0);
		require(edit->getTextSelectionLength() == 2, "Reverse selection length must use ordered bounds");
		edit->setTextCursor(2);
		require(edit->getTextSelectionLength() == 0, "An inactive selection must have zero length");
		edit->setTextSelection(0, 2);
		unittest::keyStroke(MyGUI::KeyCode::X, 'x');
		require(
			edit->getOnlyText() == "x" && edit->getTextCursor() == 1,
			"Typing must replace selection and advance the cursor");
		require(!edit->isTextSelection(), "Typing must clear the replaced selection");
		edit->setTextCursor(0);
		unittest::keyStroke(MyGUI::KeyCode::Backspace);
		edit->setTextCursor(1);
		unittest::keyStroke(MyGUI::KeyCode::Delete);
		require(edit->getOnlyText() == "x", "Backspace at start and Delete at end must be harmless");
	}

	void testHistory()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		edit->setOnlyText("abcd");
		edit->setTextCursor(3);
		unittest::keyStroke(MyGUI::KeyCode::Backspace);
		require(
			edit->getOnlyText() == "abd" && edit->getTextCursor() == 2,
			"Backspace must remove the preceding character");
		unittest::shortcut(MyGUI::KeyCode::Z);
		require(edit->getOnlyText() == "abcd" && edit->getTextCursor() == 3, "Undo must restore text and cursor");
		unittest::redo();
		require(edit->getOnlyText() == "abd" && edit->getTextCursor() == 2, "Redo must restore the edit and cursor");
		unittest::shortcut(MyGUI::KeyCode::Z);
		unittest::keyStroke(MyGUI::KeyCode::X, 'x');
		unittest::redo();
		require(edit->getOnlyText() == "abcxd", "A new edit after undo must invalidate redo");
	}

	void testReadOnlyAndLength()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		edit->setOnlyText("abc");
		edit->setEditReadOnly(true);
		edit->setTextSelection(0, 3);
		unittest::keyStroke(MyGUI::KeyCode::X, 'x');
		unittest::keyStroke(MyGUI::KeyCode::Delete);
		unittest::shortcut(MyGUI::KeyCode::X);
		require(edit->getOnlyText() == "abc", "Read-only mode must reject typing, deletion, and cut");
		require(edit->getTextSelectionLength() == 3, "Read-only text must remain selectable");
		edit->setEditReadOnly(false);
		edit->setOnlyText("");
		edit->setMaxTextLength(3);
		for (const char ch : std::string("abcd"))
			unittest::keyStroke(MyGUI::KeyCode::None, ch);
		require(edit->getOnlyText() == "abc", "Typing past maximum length must not append surplus characters");
		edit->setTextSelection(1, 3);
		unittest::keyStroke(MyGUI::KeyCode::X, 'x');
		require(edit->getOnlyText() == "ax", "Replacement must remain possible when the field was full");
	}

	void testDeleteAndNewlineHistory()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		const MyGUI::UString text(std::u32string(U"a\U0001F600bc"));
		edit->setOnlyText(text);
		edit->setTextCursor(1);
		unittest::keyStroke(MyGUI::KeyCode::Delete);
		require(
			edit->getOnlyText() == "abc" && edit->getTextCursor() == 1,
			"Delete must remove a whole supplementary character without moving the cursor");
		unittest::shortcut(MyGUI::KeyCode::Z);
		require(edit->getOnlyText() == text, "Undo must restore the complete deleted character");
		unittest::redo();
		require(edit->getOnlyText() == "abc" && edit->getTextCursor() == 1, "Redo must repeat forward deletion");

		edit->setEditMultiLine(true);
		edit->setOnlyText("abcdef");
		edit->setTextSelection(4, 1);
		unittest::keyStroke(MyGUI::KeyCode::Return);
		require(
			edit->getOnlyText() == "a\nef" && edit->getTextCursor() == 2 && !edit->isTextSelection(),
			"Enter must replace a reverse selection with one newline");
		unittest::shortcut(MyGUI::KeyCode::Z);
		require(edit->getOnlyText() == "abcdef", "One undo must restore the entire newline replacement");
		unittest::redo();
		require(
			edit->getOnlyText() == "a\nef" && edit->getTextCursor() == 2,
			"One redo must repeat the entire newline replacement");
	}

	void testCursorBoundaries()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		edit->setOnlyText(MyGUI::UString(std::u32string(U"a\U0001F600bc")));
		edit->setTextCursor(100);
		require(edit->getTextCursor() == 4, "An oversized cursor index must clamp to the logical text length");
		edit->setTextSelection(100, 1);
		require(
			edit->getTextSelectionStart() == 1 && edit->getTextSelectionEnd() == 4 && edit->getTextCursor() == 1,
			"An oversized reverse-selection start must clamp while retaining its active end");
		edit->setTextSelection(1, 100);
		require(
			edit->getTextSelectionEnd() == 4 && edit->getTextCursor() == 4,
			"An oversized selection end must clamp together with the cursor");
		edit->setTextCursor(4);
		require(!edit->isTextSelection(), "Setting the existing cursor position must still clear selection");
		edit->setTextCursor(1);
		unittest::keyStroke(MyGUI::KeyCode::ArrowRight);
		require(edit->getTextCursor() == 2, "Right must cross a supplementary character in one step");
		unittest::keyStroke(MyGUI::KeyCode::ArrowLeft);
		require(edit->getTextCursor() == 1, "Left must cross a supplementary character in one step");

		edit->setEditMultiLine(true);
		edit->setOnlyText("abcd\nefgh\nijkl");
		edit->setTextCursor(7);
		unittest::keyStroke(MyGUI::KeyCode::Home);
		require(edit->getTextCursor() == 5, "Home must move to the current line's beginning");
		auto& input = MyGUI::InputManager::getInstance();
		input.injectKeyPress(MyGUI::KeyCode::LeftShift);
		unittest::keyStroke(MyGUI::KeyCode::End);
		input.injectKeyRelease(MyGUI::KeyCode::LeftShift);
		require(
			edit->getTextCursor() == 9 && MyGUI::TextIterator::getOnlyText(edit->getTextSelection()) == "efgh",
			"Shift+End must select the current line without its newline");
		edit->setTextCursor(2);
		unittest::keyStroke(MyGUI::KeyCode::ArrowUp);
		require(edit->getTextCursor() == 0, "Up on the first line must clamp to the text beginning");
		edit->setTextCursor(12);
		input.injectKeyPress(MyGUI::KeyCode::LeftShift);
		unittest::keyStroke(MyGUI::KeyCode::ArrowDown);
		input.injectKeyRelease(MyGUI::KeyCode::LeftShift);
		require(
			edit->getTextCursor() == 14 && MyGUI::TextIterator::getOnlyText(edit->getTextSelection()) == "kl",
			"Shift+Down on the last line must select through the text end");
		unittest::keyStroke(MyGUI::KeyCode::ArrowDown);
		require(
			edit->getTextCursor() == 14 && !edit->isTextSelection(),
			"Down at the text end without Shift must clear the selection");
	}

	void testOverflowInsertionAndSelectionDeletion()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		edit->setMaxTextLength(4);
		edit->setOverflowToTheLeft(true);
		edit->setOnlyText("abcd");
		edit->addText(MyGUI::UString(std::u32string(U"\U0001F600##")));
		require(
			edit->getOnlyText().asUTF32() == U"cd\U0001F600#" && edit->getTextLength() == 4,
			"Appending past the limit must trim from the beginning without splitting Unicode or escaped hashes");
		edit->setTextSelection(3, 1);
		edit->deleteTextSelection();
		require(
			edit->getOnlyText() == "c#" && edit->getTextCursor() == 1 && !edit->isTextSelection(),
			"Public deletion of a reverse selection must preserve surrounding text and collapse the cursor");
		edit->deleteTextSelection();
		require(edit->getOnlyText() == "c#", "Deleting without a selection must leave the text unchanged");
	}

	void testStaticMode()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		edit->setOnlyText("abcdef");
		edit->setTextCursor(2);
		const std::string pointer(edit->getClientWidget()->getPointer());
		edit->setEditStatic(true);
		unittest::keyStroke(MyGUI::KeyCode::X, 'x');
		unittest::keyStroke(MyGUI::KeyCode::Delete);
		unittest::keyStroke(MyGUI::KeyCode::ArrowRight);
		unittest::dragFromTo(MyGUI::IntPoint(11, 10), MyGUI::IntPoint(41, 10));
		context.getGui().eventFrameStart(1.0f);
		require(
			edit->getOnlyText() == "abcdef" && edit->getTextCursor() == 2 && !edit->isTextSelection(),
			"Static mode must reject editing, keyboard navigation and mouse selection");
		edit->setEditStatic(false);
		require(
			edit->getClientWidget()->getPointer() == pointer,
			"Leaving static mode must restore the client pointer");
		MyGUI::InputManager::getInstance().setKeyFocusWidget(edit);
		unittest::keyStroke(MyGUI::KeyCode::X, 'x');
		require(
			edit->getOnlyText() == "abxcdef" && edit->getTextCursor() == 3,
			"Leaving static mode must restore normal keyboard editing");
	}

	void testUnicodeAndTags()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		const MyGUI::UString unicode("A\xF0\x9F\x98\x80\xD0\x91"); // A, U+1F600, U+0411
		edit->setCaption(MyGUI::UString("#FF0000") + unicode);
		require(
			edit->getOnlyText() == unicode && edit->getTextLength() == 3,
			"Text length must count Unicode characters and exclude colour tags");
		edit->setTextCursor(2);
		unittest::keyStroke(MyGUI::KeyCode::Backspace);
		require(edit->getOnlyText() == MyGUI::UString("A\xD0\x91"), "Backspace must remove a whole non-BMP character");
		unittest::shortcut(MyGUI::KeyCode::Z);
		require(edit->getOnlyText() == unicode, "Undo must restore the complete Unicode character");
		edit->setOnlyText("");
		for (const auto character : std::u32string(U"\U0001F600\U0001D800#"))
			unittest::keyStroke(MyGUI::KeyCode::None, character);
		require(
			edit->getOnlyText().asUTF32() == U"\U0001F600\U0001D800#" && edit->getTextCursor() == 3,
			"Typing must preserve full code points and escape hashes");
		unittest::keyStroke(MyGUI::KeyCode::Backspace);
		unittest::keyStroke(MyGUI::KeyCode::Backspace);
		unittest::shortcut(MyGUI::KeyCode::Z);
		require(
			edit->getOnlyText().asUTF32() == U"\U0001F600\U0001D800",
			"Undo must restore a typed supplementary character whose low bits form a surrogate");
		edit->setOnlyText("#FF0000literal#");
		require(edit->getOnlyText() == "#FF0000literal#", "setOnlyText must preserve literal colour-like text");
		edit->setMaxTextLength(1);
		edit->setOverflowToTheLeft(true);
		edit->setOnlyText("text#");
		require(edit->getOnlyText() == "#", "Left overflow must preserve an escaped trailing hash");
	}

	struct Clipboard
	{
		std::string text{"sentinel"};
		void changed(std::string_view, std::string_view _text)
		{
			text = _text;
		}
		void requested(std::string_view, std::string& _text)
		{
			_text = text;
		}
	};

	void testClipboardAndPassword()
	{
		Clipboard clipboard;
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		auto& manager = MyGUI::ClipboardManager::getInstance();
		// Replace platform clipboard callbacks so tests never read or write the desktop clipboard.
		manager.eventClipboardChanged.clear();
		manager.eventClipboardRequested.clear();
		manager.eventClipboardChanged += MyGUI::newDelegate(&clipboard, &Clipboard::changed);
		manager.eventClipboardRequested += MyGUI::newDelegate(&clipboard, &Clipboard::requested);
		edit->setOnlyText("hello");
		edit->setTextSelection(1, 4);
		unittest::shortcut(MyGUI::KeyCode::C);
		require(
			MyGUI::TextIterator::getOnlyText(MyGUI::UString(clipboard.text)) == "ell",
			"Copy must contain only selected text");
		unittest::shortcut(MyGUI::KeyCode::X);
		require(edit->getOnlyText() == "ho", "Cut must remove selected text");
		unittest::shortcut(MyGUI::KeyCode::V);
		require(edit->getOnlyText() == "hello", "Paste must insert clipboard text at the cursor");
		edit->setEditPassword(true);
		edit->setTextSelection(0, 5);
		clipboard.text = "sentinel";
		unittest::shortcut(MyGUI::KeyCode::C);
		require(
			clipboard.text.empty() || clipboard.text == "sentinel",
			"Copying a password must not expose its contents");
		unittest::shortcut(MyGUI::KeyCode::X);
		require(
			clipboard.text.empty() || clipboard.text == "sentinel",
			"Cutting a password must not expose its contents");
	}

	void testNewlines()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		edit->setOnlyText("a\nb");
		require(edit->getOnlyText() == "a b", "Single-line fields must replace newlines with spaces");
		edit->setEditMultiLine(true);
		edit->setOnlyText("a\nb");
		require(edit->getOnlyText() == "a\nb", "Multiline fields must retain newlines");
		edit->setTextCursor(1);
		unittest::keyStroke(MyGUI::KeyCode::Return);
		require(edit->getOnlyText() == "a\n\nb", "Enter in multiline mode must insert a newline");

		edit->setOnlyText("");
		edit->setEditPassword(true);
		edit->setEditMultiLine(true);
		require(!edit->getEditMultiLine(), "Password fields must remain single-line");
		unittest::keyStroke(MyGUI::KeyCode::Return);
		require(edit->getOnlyText().empty(), "Enter in password mode must not insert a newline");
	}

	void testWordNavigation()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		edit->setCaption(MyGUI::UString(std::u32string(U"#FF0000a\U0001F600 #00FF00b##c")));
		edit->setTextCursor(0);
		auto& input = MyGUI::InputManager::getInstance();
#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
		const auto modifier = MyGUI::KeyCode::LeftAlt;
#else
		const auto modifier = MyGUI::KeyCode::LeftControl;
#endif
		input.injectKeyPress(modifier);
		unittest::keyStroke(MyGUI::KeyCode::ArrowRight);
		require(edit->getTextCursor() == 2, "Word movement must count supplementary characters and skip colour markup");
		unittest::keyStroke(MyGUI::KeyCode::ArrowRight);
		require(edit->getTextCursor() == 6, "Word movement must count an escaped hash once");
		unittest::keyStroke(MyGUI::KeyCode::ArrowLeft);
		require(edit->getTextCursor() == 3, "Reverse word movement must use logical text positions");
		unittest::keyStroke(MyGUI::KeyCode::ArrowLeft);
		require(edit->getTextCursor() == 0, "Reverse word movement must preserve supplementary character boundaries");
		input.injectKeyRelease(modifier);
	}

	void testDoubleClickUnicodeWord()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		edit->setCaption(MyGUI::UString(std::u32string(U"#FF0000one \U0001F600two end")));
		auto& input = MyGUI::InputManager::getInstance();
		input.setDoubleClickTime(0.5f);
		input.injectMouseMove(55, 10, 0);
		input.injectMousePress(55, 10, MyGUI::MouseButton::Left);
		input.injectMouseRelease(55, 10, MyGUI::MouseButton::Left);
		context.getGui().eventFrameStart(0.1f);
		input.injectMousePress(55, 10, MyGUI::MouseButton::Left);
		input.injectMouseRelease(55, 10, MyGUI::MouseButton::Left);
		require(
			edit->getTextSelectionStart() == 4 && edit->getTextSelectionLength() == 4,
			"Double-click selection must use the complete Unicode word and exclude colour markup");
		require(
			MyGUI::TextIterator::getOnlyText(edit->getTextSelection()).asUTF32() == U"\U0001F600two",
			"Double-click selection must preserve supplementary characters");
	}

	void testPasswordCharacter()
	{
		unittest::TestContext context;
		auto* edit = createEdit(context.getGui());
		edit->setOnlyText("abc");
		edit->setPasswordChar(MyGUI::UString(std::u32string(U"\U0001D800")));
		require(edit->getPasswordChar() == 0x1D800, "A password character must use the full Unicode code point");
		edit->setEditPassword(true);
		require(
			edit->getClientWidget()->getSubWidgetText()->getCaption().asUTF32() == U"\U0001D800\U0001D800\U0001D800",
			"Password masking must encode each complete character");
		edit->setPasswordChar(0x1F600);
		require(
			edit->getClientWidget()->getSubWidgetText()->getCaption().asUTF32() == U"\U0001F600\U0001F600\U0001F600",
			"Changing the password character must update the complete mask");
		require(edit->getOnlyText() == "abc", "Password masking must preserve the original text");
		edit->setTextCursor(1);
		unittest::keyStroke(MyGUI::KeyCode::X, 'x');
		edit->setEditPassword(true);
		require(edit->getOnlyText() == "axbc", "Repeated password enabling must preserve edits to the hidden text");
		edit->setEditPassword(false);
		require(
			edit->getOnlyText() == "axbc" && edit->getClientWidget()->getSubWidgetText()->getCaption() == "axbc",
			"Disabling password mode must display the edited original text instead of the mask");
		edit->setEditPassword(false);
		edit->setEditPassword(true);
		edit->setEditPassword(false);
		require(edit->getOnlyText() == "axbc", "Repeated password transitions must not lose the original text");
	}

}

int main()
{
	return unittest::runTests({
		{"Colour ranges across markup and Unicode", testEditColours},
		{"Empty and oversized colour ranges", testEditColourBoundaries},
		{"Page, document-boundary and password navigation", testEditKeyboardNavigation},
		{"Select-all, tabs, acceptance and Escape", testEditKeyboardCommands},
		{"Mouse, vertical navigation and scroll synchronization", testEditBoxInteractions},
		{"Held drag autoscroll at all four edges", testEditBoxDragAutoscroll},
		{"Double-click Unicode word", testDoubleClickUnicodeWord},
		{"Unicode word navigation", testWordNavigation},
		{"Supplementary password character", testPasswordCharacter},
		{"Text intervals and replacement", testIntervals},
		{"Undo, redo, and history branching", testHistory},
		{"Forward deletion and newline replacement history", testDeleteAndNewlineHistory},
		{"Cursor and selection boundaries", testCursorBoundaries},
		{"Overflow insertion and public selection deletion", testOverflowInsertionAndSelectionDeletion},
		{"Static mode and editing recovery", testStaticMode},
		{"Read-only and maximum length", testReadOnlyAndLength},
		{"Unicode and colour tags", testUnicodeAndTags},
		{"Clipboard and password", testClipboardAndPassword},
		{"Single-line and multiline input", testNewlines},
	});
}
