#include "BehaviourTestSupport.h"
#include "FixedFont.h"

namespace
{

	using unittest::require;

	MyGUI::EditBox* createEdit(MyGUI::Gui& _gui)
	{
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
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
	}

}

int main()
{
	return unittest::runTests({
		{"Supplementary password character", testPasswordCharacter},
		{"Text intervals and replacement", testIntervals},
		{"Undo, redo, and history branching", testHistory},
		{"Read-only and maximum length", testReadOnlyAndLength},
		{"Unicode and colour tags", testUnicodeAndTags},
		{"Clipboard and password", testClipboardAndPassword},
		{"Single-line and multiline input", testNewlines},
	});
}
