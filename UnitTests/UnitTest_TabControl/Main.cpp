/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI.h"
#include "BehaviourTestSupport.h"
#include "FixedFont.h"
#include <vector>

namespace tabcontrol_test
{

	using unittest::require;

	class TestTabControl : public MyGUI::TabControl
	{
		MYGUI_RTTI_DERIVED(TestTabControl)

	public:
		using MyGUI::Widget::assignWidget;
	};

	struct Fixture
	{
		Fixture()
		{
			unittest::registerFixedFont();
			MyGUI::FactoryManager::getInstance().registerFactory<TestTabControl>(
				MyGUI::WidgetManager::getInstance().getCategoryName());
			MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
			unittest::loadResources("UnitTest_TabControl/TestSkin.xml");
		}

		unittest::TestContext context;
	};

	struct SelectionEvents
	{
		std::vector<size_t> indices;
		void changed(MyGUI::TabControl* _sender, size_t _index)
		{
			require(_sender->getIndexSelected() == _index, "Selection must be updated before notification");
			indices.push_back(_index);
		}
	};

	static TestTabControl* createTabs(MyGUI::Gui& _gui, const std::string& _skin)
	{
		auto* tabs =
			_gui.createWidget<TestTabControl>(_skin, MyGUI::IntCoord(20, 30, 240, 120), MyGUI::Align::Default, "Main");
		tabs->setSmoothShow(false);
		tabs->setButtonDefaultWidth(80);
		return tabs;
	}

	static void checkSelection(MyGUI::TabControl* _tabs, size_t _index)
	{
		require(_tabs->getIndexSelected() == _index, "Unexpected selected tab index");
		require(_tabs->getItemSelected() == _tabs->getItemAt(_index), "Selected page must match its index");
		for (size_t index = 0; index < _tabs->getItemCount(); ++index)
			require(
				_tabs->getItemAt(index)->getVisible() == (index == _index),
				"Only the selected page must be visible");
	}

	template<bool OldSkin>
	static void testItems()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		const std::string skin = OldSkin ? "TestTabControlOld" : "TestTabControl";
		auto* tabs = createTabs(gui, skin);
		require(
			tabs->getItemCount() == 0 && tabs->getIndexSelected() == MyGUI::ITEM_NONE &&
				tabs->getItemSelected() == nullptr,
			"New tabs must have no selection");
		tabs->beginToItemFirst();
		tabs->beginToItemLast();
		tabs->beginToItemSelected();
		auto* first = tabs->addItem("First", 10);
		auto* second = tabs->addItem("Second", 20);
		auto* third = tabs->addItem("Third", 30);
		checkSelection(tabs, 0);
		require(first->getCoord() == MyGUI::IntCoord(0, 20, 240, 100), "Pages must use the skin's client template");
		require(
			tabs->findItemIndexWith("Second") == 1 && tabs->findItemWith("Third") == third,
			"Name lookup must find the matching page");
		require(
			tabs->findItemIndexWith("Missing") == MyGUI::ITEM_NONE && tabs->findItemWith("Missing") == nullptr &&
				tabs->findItemIndex(nullptr) == MyGUI::ITEM_NONE,
			"Missing pages must return the documented sentinels");
		second->setCaption("Renamed");
		require(
			tabs->getItemName(second) == "Renamed" && second->getCaption() == "Renamed",
			"TabItem captions must update the control's item names");
		tabs->setItemNameAt(1, "First");
		require(tabs->findItemWith("First") == first, "Duplicate name lookup must return the first match");
		require(*tabs->getItemData<int>(second) == 20, "Insertion must retain item data");
		tabs->setItemData(second, std::string("payload"));
		require(
			*tabs->getItemDataAt<std::string>(1) == "payload" && tabs->getItemData<int>(second, false) == nullptr,
			"Replacing data must update its type and value");
		tabs->clearItemData(second);
		tabs->clearItemDataAt(0);
		require(
			tabs->getItemDataAt<int>(0, false) == nullptr && tabs->getItemData<std::string>(second, false) == nullptr,
			"Clearing item data must remove the stored values");

		SelectionEvents events;
		tabs->eventTabChangeSelect += MyGUI::newDelegate(&events, &SelectionEvents::changed);
		tabs->setItemSelected(third);
		checkSelection(tabs, 2);
		tabs->setIndexSelected(2);
		require(events.indices.empty(), "Programmatic selection must not emit a user selection event");
		tabs->removeItem(first);
		checkSelection(tabs, 1);
		require(tabs->getItemSelected() == third, "Removing an earlier tab must preserve the selected page");
		gui.destroyWidget(third);
		checkSelection(tabs, 0);
		require(tabs->getItemSelected() == second, "Direct page destruction must update the owning control");
		tabs->removeAllItems();
		require(
			tabs->getItemCount() == 0 && tabs->getItemSelected() == nullptr &&
				tabs->getIndexSelected() == MyGUI::ITEM_NONE,
			"Removing all pages must clear selection");
		tabs->removeAllItems();
		tabs->addItem("Reused");
		checkSelection(tabs, 0);
		gui.destroyWidget(tabs);
	}

	template<bool OldSkin>
	static void testInsertionAndWidths()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		const std::string skin = OldSkin ? "TestTabControlOld" : "TestTabControl";
		auto* tabs = createTabs(gui, skin);
		auto* first = tabs->addItem("First", 1);
		auto* last = tabs->addItem("Last", 3);
		auto* middle = tabs->insertItem(last, "Middle", 2);
		require(
			tabs->getItemAt(0) == first && tabs->getItemAt(1) == middle && tabs->getItemAt(2) == last,
			"Inserting before the last page must place the new page before it");
		tabs->swapItems(1, 2);
		require(
			tabs->getItemAt(1) == last && *tabs->getItemDataAt<int>(2) == 2,
			"Swapping pages must move their names and data together");
		tabs->swapItems(1, 1);
		checkSelection(tabs, 0);
		middle->setButtonWidth(100);
		require(tabs->getButtonWidth(middle) == 100, "TabItem must forward header width changes");
		tabs->setButtonWidth(middle);
		require(tabs->getButtonWidth(middle) == 80, "Default width must restore the configured fixed width");
		tabs->setProperty("ButtonWidth", "60");
		require(
			tabs->getButtonDefaultWidth() == 60 && !tabs->getButtonAutoWidth(),
			"Setting the default width must disable automatic widths");
		for (size_t index = 0; index < tabs->getItemCount(); ++index)
			require(tabs->getButtonWidthAt(index) == 60, "Default width changes must resize existing headers");
		tabs->setButtonDefaultWidth(0);
		require(
			tabs->getButtonDefaultWidth() == 1 && tabs->getButtonWidthAt(0) == 1,
			"Nonpositive default widths must clamp to one");
		tabs->setProperty("ButtonAutoWidth", "true");
		require(tabs->getButtonAutoWidth(), "The automatic width property must enable automatic widths");
		tabs->setProperty("SelectItem", "1");
		checkSelection(tabs, 1);
		tabs->setSize(300, 160);
		require(first->getCoord() == MyGUI::IntCoord(0, 20, 300, 140), "Pages must stretch with their control");
		tabs->setPosition(40, 50);
		require(first->getAbsolutePosition() == MyGUI::IntPoint(40, 70), "Pages must follow control movement");
		gui.destroyWidget(tabs);
	}

	template<bool OldSkin, bool Smooth>
	static void testHeaders()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		const std::string skin = OldSkin ? "TestTabControlOld" : "TestTabControl";
		auto* tabs = createTabs(gui, skin);
		tabs->setProperty("SmoothShow", Smooth ? "true" : "false");
		require(tabs->getSmoothShow() == Smooth, "The smooth selection property must be applied");
		for (int index = 0; index < 5; ++index)
			tabs->addItem(MyGUI::utility::toString(index));
		MyGUI::Widget* bar = nullptr;
		MyGUI::Button* left = nullptr;
		MyGUI::Button* right = nullptr;
		tabs->assignWidget(bar, skin == "TestTabControl" ? "HeaderPlace" : "Bar");
		tabs->assignWidget(left, "Left");
		tabs->assignWidget(right, "Right");
		require(bar && left && right, "The fixture must expose its header bar and navigation buttons");
		require(!left->getEnabled() && right->getEnabled(), "Overflow must initially allow scrolling only right");
		auto header = [&](size_t _slot)
		{
			// Skin children are separate from the dynamically created header buttons.
			size_t slot = 0;
			for (size_t index = 0; index < bar->getChildCount(); ++index)
			{
				auto* button = bar->getChildAt(index)->castType<MyGUI::Button>(false);
				if (button && button->getVisible() && slot++ == _slot)
					return button;
			}
			require(false, "Expected a visible header button");
			return static_cast<MyGUI::Button*>(nullptr);
		};
		SelectionEvents events;
		tabs->eventTabChangeSelect += MyGUI::newDelegate(&events, &SelectionEvents::changed);
		auto* button = header(1);
		button->eventMouseButtonClick(button);
		gui.eventFrameStart(1.0f);
		checkSelection(tabs, 1);
		require(
			button->getStateSelected() && events.indices == std::vector<size_t>{1},
			"Clicking a header must select it and emit one event");
		button->eventMouseButtonClick(button);
		require(events.indices.size() == 1, "Clicking the selected header must not emit another event");
		right->eventMouseButtonClick(right);
		require(
			left->getEnabled() && header(0)->getCaption() == "1",
			"The right arrow must advance the visible headers");
		left->eventMouseButtonClick(left);
		require(!left->getEnabled() && header(0)->getCaption() == "0", "The left arrow must restore the first header");
		tabs->beginToItemLast();
		require(
			!right->getEnabled() && header(1)->getCaption() == "4",
			"Scrolling to the last tab must reveal its header");
		tabs->beginToItem(tabs->getItemAt(0));
		require(header(0)->getCaption() == "0", "Scrolling to a page must reveal its header");
		tabs->setCoord(20, 30, 480, 120);
		require(header(4)->getCaption() == "4", "Growing the control must reveal all headers");
		checkSelection(tabs, 1);
		tabs->setIndexSelected(2);
		tabs->setIndexSelected(1);
		gui.eventFrameStart(1.0f);
		checkSelection(tabs, 1);
		require(tabs->getItemSelected()->getEnabled(), "Rapid switching must leave the active page enabled");
		tabs->setIndexSelected(3);
		gui.destroyWidget(tabs);
		gui.eventFrameStart(1.0f);
	}

}

int main()
{
	return unittest::runTests({
		{"Items", tabcontrol_test::testItems<false>},
		{"Insertion and widths", tabcontrol_test::testInsertionAndWidths<false>},
		{"Headers", tabcontrol_test::testHeaders<false, false>},
		{"Headers with smooth show", tabcontrol_test::testHeaders<false, true>},
		{"Items (old skin)", tabcontrol_test::testItems<true>},
		{"Insertion and widths (old skin)", tabcontrol_test::testInsertionAndWidths<true>},
		{"Headers (old skin)", tabcontrol_test::testHeaders<true, false>},
		{"Headers with smooth show (old skin)", tabcontrol_test::testHeaders<true, true>},
	});
}
