#include "Interaction.h"
#include "BehaviourTestSupport.h"
#include "FixedFont.h"
#include "SkinTestContext.h"

namespace interaction
{

	class InteractiveEdit : public MyGUI::EditBox
	{
		MYGUI_RTTI_DERIVED(InteractiveEdit)
	public:
		MyGUI::ScrollBar* scroll(std::string_view _name)
		{
			MyGUI::ScrollBar* result = nullptr;
			assignWidget(result, _name);
			return result;
		}
	};

}

void testEditBoxDragAutoscroll()
{
	using unittest::require;
	unittest::SkinTestContext context;
	context.loadSkins();
	context.loadMedia("MyGUI_BlueWhiteTemplates.xml");
	unittest::registerFixedFont();
	unittest::createInputLayer();
	MyGUI::FactoryManager::getInstance().registerFactory<interaction::InteractiveEdit>(
		MyGUI::WidgetManager::getInstance().getCategoryName());
	auto& gui = MyGUI::Gui::getInstance();
	auto& input = MyGUI::InputManager::getInstance();
	for (int edge = 0; edge < 4; ++edge)
	{
		auto* edit = gui.createWidget<interaction::InteractiveEdit>(
			"EditBoxStretch",
			MyGUI::IntCoord(100, 100, 180, 100),
			MyGUI::Align::Default,
			"Main");
		edit->setFontName("UnitTestFixedFont");
		edit->setFontHeight(20);
		edit->setTextAlign(MyGUI::Align::Left | MyGUI::Align::Top);
		edit->setEditMultiLine(true);
		std::string text;
		for (int i = 0; i < 30; ++i)
			text += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n";
		edit->setOnlyText(text);
		input.setKeyFocusWidget(edit);
		edit->setTextCursor(53 * 15 + 25);
		edit->setVScrollPosition(250);
		edit->setHScrollPosition(150);
		const auto view = edit->getClientWidget()->getAbsoluteRect();
		const MyGUI::IntPoint centre((view.left + view.right) / 2, (view.top + view.bottom) / 2);
		MyGUI::IntPoint outside = centre;
		if (edge == 0)
			outside.top = view.top - 5;
		if (edge == 1)
			outside.top = view.bottom + 5;
		if (edge == 2)
			outside.left = view.left - 5;
		if (edge == 3)
			outside.left = view.right + 5;
		input.injectMouseMove(centre.left, centre.top, 0);
		input.injectMousePress(centre.left, centre.top, MyGUI::MouseButton::Left);
		input.injectMouseMove(outside.left, outside.top, 0);
		const auto cursor = edit->getTextCursor();
		const auto offset = edge < 2 ? edit->getVScrollPosition() : edit->getHScrollPosition();
		for (int frame = 0; frame < 5; ++frame)
			gui.eventFrameStart(0.1f);
		const auto moved = edit->getTextCursor();
		const auto scrolled = edge < 2 ? edit->getVScrollPosition() : edit->getHScrollPosition();
		require(
			(edge == 0 || edge == 2) ? moved < cursor && scrolled < offset : moved > cursor && scrolled > offset,
			"Holding a drag beyond an edge must extend the cursor and scroll in that direction");
		require(edit->isTextSelection(), "Autoscroll must retain an active selection");
		require(
			edit->getVScrollPosition() < edit->getVScrollRange() &&
				edit->getHScrollPosition() < edit->getHScrollRange() &&
				edit->scroll("VScroll")->getScrollPosition() == edit->getVScrollPosition() &&
				edit->scroll("HScroll")->getScrollPosition() == edit->getHScrollPosition(),
			"Autoscroll offsets must stay bounded and synchronized with both scrollbars");
		input.injectMouseRelease(outside.left, outside.top, MyGUI::MouseButton::Left);
		const auto start = edit->getTextSelectionStart(), end = edit->getTextSelectionEnd();
		const auto vertical = edit->getVScrollPosition(), horizontal = edit->getHScrollPosition();
		gui.eventFrameStart(1.0f);
		require(
			edit->getTextCursor() == moved && edit->getTextSelectionStart() == start &&
				edit->getTextSelectionEnd() == end && edit->getVScrollPosition() == vertical &&
				edit->getHScrollPosition() == horizontal,
			"Releasing the mouse must stop autoscroll without changing the selection");
		gui.destroyWidget(edit);
	}
}

void testEditBoxInteractions()
{
	using unittest::require;
	using interaction::InteractiveEdit;
	unittest::SkinTestContext context;
	context.loadSkins();
	context.loadMedia("MyGUI_BlueWhiteTemplates.xml");
	unittest::registerFixedFont();
	unittest::createInputLayer();
	MyGUI::FactoryManager::getInstance().registerFactory<InteractiveEdit>(
		MyGUI::WidgetManager::getInstance().getCategoryName());
	auto* edit = MyGUI::Gui::getInstance().createWidget<InteractiveEdit>(
		"EditBoxStretch",
		MyGUI::IntCoord(20, 20, 180, 100),
		MyGUI::Align::Default,
		"Main");
	edit->setFontName("UnitTestFixedFont");
	edit->setFontHeight(20);
	edit->setTextAlign(MyGUI::Align::Left | MyGUI::Align::Top);
	edit->setEditMultiLine(true);
	edit->setOnlyText("abcdef\nabcdef\nabcdef");
	auto& input = MyGUI::InputManager::getInstance();
	input.setKeyFocusWidget(edit);
	edit->setTextCursor(2);
	unittest::keyStroke(MyGUI::KeyCode::ArrowDown);
	require(edit->getTextCursor() == 9, "Down must preserve the column on the next line");
	unittest::keyStroke(MyGUI::KeyCode::ArrowUp);
	require(edit->getTextCursor() == 2, "Up must return to the same column");
	input.injectKeyPress(MyGUI::KeyCode::LeftShift);
	unittest::keyStroke(MyGUI::KeyCode::ArrowDown);
	input.injectKeyRelease(MyGUI::KeyCode::LeftShift);
	require(
		edit->getTextSelectionStart() == 2 && edit->getTextSelectionLength() == 7,
		"Shift+Down must select across the newline");
	edit->setTextCursor(0);
	const auto origin = edit->getClientWidget()->getAbsolutePosition();
	unittest::dragFromTo(origin + MyGUI::IntPoint(11, 10), origin + MyGUI::IntPoint(41, 10));
	require(
		edit->getTextSelectionStart() == 1 && edit->getTextSelectionLength() == 3,
		"Dragging must select exactly the traversed characters");
	require(MyGUI::TextIterator::getOnlyText(edit->getTextSelection()) == "bcd", "Drag selection text must match");
	MyGUI::Gui::getInstance().eventFrameStart(1.0f);
	unittest::dragFromTo(origin + MyGUI::IntPoint(51, 10), origin + MyGUI::IntPoint(21, 10));
	require(
		edit->getTextSelectionStart() == 2 && edit->getTextSelectionLength() == 3,
		"Reverse drag must order selection bounds");
	std::string longText;
	for (int i = 0; i < 20; ++i)
		longText += "abcdefghijklmnopqrstuvwxyz\n";
	edit->setOnlyText(longText);
	edit->setTextCursor(0);
	edit->setVScrollPosition(0);
	edit->setHScrollPosition(0);
	auto* vertical = edit->scroll("VScroll");
	auto* horizontal = edit->scroll("HScroll");
	require(
		vertical && horizontal && edit->getVScrollRange() > 1 && edit->getHScrollRange() > 1,
		"Long multiline text must require both scrollbars");
	input.injectMouseMove(origin.left + 5, origin.top + 5, -1);
	require(
		edit->getVScrollPosition() == 50 && vertical->getScrollPosition() == 50,
		"Wheel input must scroll the text and vertical bar together");
	input.injectMouseMove(origin.left + 5, origin.top + 5, 0);
	require(
		edit->getVScrollPosition() == 0 && vertical->getScrollPosition() == 0,
		"Reverse wheel input must return the text and vertical bar to the top");
	input.injectMouseMove(origin.left + 5, origin.top + 5, 1);
	require(edit->getVScrollPosition() == 0, "Reverse wheel input at the top must clamp to zero");
	vertical->setScrollPosition(30);
	vertical->eventScrollChangePosition(vertical, 30);
	horizontal->setScrollPosition(20);
	horizontal->eventScrollChangePosition(horizontal, 20);
	require(
		edit->getVScrollPosition() == 30 && edit->getHScrollPosition() == 20,
		"Scrollbar notifications must update both text offsets independently");
	edit->setVScrollPosition(edit->getVScrollRange() + 100);
	edit->setHScrollPosition(edit->getHScrollRange() + 100);
	require(
		vertical->getScrollPosition() == edit->getVScrollPosition() &&
			horizontal->getScrollPosition() == edit->getHScrollPosition(),
		"Clamped API scrolling must synchronize both bars");
	edit->setOnlyText("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
	edit->setTextCursor(0);
	edit->setHScrollPosition(0);
	input.injectMouseMove(origin.left + 5, origin.top + 5, 0);
	require(
		edit->getVScrollRange() == 1 && edit->getHScrollPosition() == 50 && horizontal->getScrollPosition() == 50,
		"Wheel must fall back to horizontal scrolling when all lines fit vertically");
	input.injectMouseMove(origin.left + 5, origin.top + 5, 1);
	require(
		edit->getHScrollPosition() == 0 && horizontal->getScrollPosition() == 0,
		"Reverse wheel input must return the text and horizontal bar to the left edge");
	input.injectMouseMove(origin.left + 5, origin.top + 5, 2);
	require(edit->getHScrollPosition() == 0, "Reverse wheel input at the left edge must clamp to zero");
}
