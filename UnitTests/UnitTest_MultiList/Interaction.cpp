#include "Interaction.h"
#include "BehaviourTestSupport.h"
#include "FixedFont.h"

void testMultiListInteractions()
{
	using unittest::require;
	std::vector<size_t> changed, accepted, pressed;
	unittest::TestContext context;
	unittest::createInputLayer();
	unittest::registerFixedFont();
	unittest::loadResources("UnitTest_MultiList/InteractionSkin.xml");
	auto& gui = context.getGui();
	auto& input = MyGUI::InputManager::getInstance();
	auto* multi = gui.createWidget<MyGUI::MultiListBox>(
		"InteractionMultiList",
		MyGUI::IntCoord(20, 20, 380, 144),
		MyGUI::Align::Default,
		"Main");
	for (const char* name : {"Name", "Value", "Details"})
		multi->addColumn(name, 120);
	for (size_t i = 0; i < 20; ++i)
	{
		const auto number = std::to_string(19 - i);
		multi->addItem(number.size() == 1 ? "0" + number : number, i);
		multi->setSubItemNameAt(1, i, MyGUI::utility::toString("Value ", i));
		multi->setSubItemNameAt(2, i, MyGUI::utility::toString("Details ", i));
	}
	multi->sortByColumn(0);
	gui.eventFrameStart(0.1f);
	auto column = [&](size_t _index)
	{
		return multi->_getItemAt(_index)->getChildAt(0)->castType<MyGUI::ListBox>();
	};
	auto row = [&](size_t _column, size_t _row)
	{
		return column(_column)->getWidgetByIndex(_row);
	};
	auto point = [&](size_t _column, size_t _row)
	{
		return row(_column, _row)->getAbsolutePosition() + MyGUI::IntPoint(5, 5);
	};
	auto hover = [&](size_t _column, size_t _row)
	{
		const auto p = point(_column, _row);
		input.injectMouseMove(p.left, p.top, 0);
	};
	auto checkColour = [&](size_t _row, const MyGUI::Colour& _colour)
	{
		for (size_t c = 0; c < 3; ++c)
			require(
				row(c, _row)->getSubWidgetText()->getTextColour() == _colour,
				"Hover styling must match across every column");
	};
	require(
		column(0)->getItemNameAt(0) == "00",
		"Fixture must sort display rows independently of logical item indices");
	hover(0, 2);
	checkColour(2, MyGUI::Colour(0.8f, 0.7f, 0.6f));
	hover(1, 3);
	checkColour(2, MyGUI::Colour(0.1f, 0.2f, 0.3f));
	checkColour(3, MyGUI::Colour(0.8f, 0.7f, 0.6f));
	input.injectMouseMove(700, 500, 0);
	checkColour(3, MyGUI::Colour(0.1f, 0.2f, 0.3f));
	multi->eventListChangePosition +=
		MyGUI::newDelegate([&](MyGUI::MultiListBox*, size_t _index) { changed.push_back(_index); }, 1);
	multi->eventListSelectAccept +=
		MyGUI::newDelegate([&](MyGUI::MultiListBox*, size_t _index) { accepted.push_back(_index); }, 2);
	multi->eventNotifyItem += MyGUI::newDelegate(
		[&](MyGUI::MultiListBox*, const MyGUI::IBNotifyItemData& _info)
		{
			if (_info.notify == MyGUI::IBNotifyItemData::MousePressed)
				pressed.push_back(_info.index);
		},
		3);
	auto checkSelection = [&](size_t _physical, size_t _logical)
	{
		require(multi->getIndexSelected() == _logical, "MultiList selection must retain logical indices after sorting");
		for (size_t c = 0; c < 3; ++c)
		{
			require(column(c)->getIndexSelected() == _physical, "Every column must select the same display row");
			require(
				row(c, _physical)->castType<MyGUI::Button>()->getStateSelected(),
				"The selected row must be visibly selected in every column");
		}
	};
	const auto first = point(0, 1);
	unittest::clickAt(first.left, first.top);
	checkSelection(1, 18);
	require(
		changed == std::vector<size_t>{18} && pressed == std::vector<size_t>{18},
		"Mouse callbacks must report logical row 18 once");
	input.setKeyFocusWidget(column(1));
	unittest::keyStroke(MyGUI::KeyCode::ArrowDown);
	checkSelection(2, 17);
	unittest::keyStroke(MyGUI::KeyCode::Return);
	require(
		changed == std::vector<size_t>({18, 17}) && accepted == std::vector<size_t>{17},
		"Keyboard selection and acceptance from another column must report the logical row once");
	hover(2, 3);
	const auto wheel = point(2, 3);
	input.injectMouseMove(wheel.left, wheel.top, -1);
	for (size_t c = 0; c < 3; ++c)
		require(column(c)->_getScrollPosition() == 20, "Wheel scrolling must synchronize every column in pixels");
	multi->setColumnWidthAt(0, 130);
	multi->swapColumnsAt(0, 2);
	require(
		multi->getColumnNameAt(0) == "Details" && multi->getColumnNameAt(2) == "Name" &&
			multi->getColumnWidthAt(2) == 130,
		"Swapping columns must move their names and resized widths together");
	checkSelection(2, 17);
	for (size_t c = 0; c < 3; ++c)
		require(
			column(c)->_getScrollPosition() == 20,
			"Column resizing and swapping must preserve synchronized scrolling");
	const auto afterSwap = point(0, 3);
	unittest::clickAt(afterSwap.left, afterSwap.top, -1);
	checkSelection(3, 16);
	require(
		changed == std::vector<size_t>({18, 17, 16}) && pressed == std::vector<size_t>({18, 16}) &&
			*multi->getSubItemDataAt<size_t>(2, 16) == 16 && column(0)->getItemNameAt(3) == "Details 16" &&
			column(2)->getItemNameAt(3) == "03",
		"After column mutations, clicks and row contents must still refer to the same logical item");
	const auto afterSwapHover = point(2, 4);
	input.injectMouseMove(afterSwapHover.left, afterSwapHover.top, -1);
	checkColour(4, MyGUI::Colour(0.8f, 0.7f, 0.6f));
	input.injectMouseMove(afterSwapHover.left, afterSwapHover.top, -2);
	for (size_t c = 0; c < 3; ++c)
		require(column(c)->_getScrollPosition() == 40, "Wheel synchronization must survive resizing and column swaps");
	input.setKeyFocusWidget(column(2));
	unittest::keyStroke(MyGUI::KeyCode::Return);
	require(
		accepted == std::vector<size_t>({17, 16}),
		"Acceptance after a column swap must keep the logical row index");
}
