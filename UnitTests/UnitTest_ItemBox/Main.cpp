#include "BehaviourTestSupport.h"
#include "TestRunner.h"
#include <vector>

namespace
{

	using unittest::require;

	struct ItemBoxFixture
	{
		std::vector<MyGUI::Widget*> created;
		std::vector<MyGUI::IBDrawItemInfo> draws;
		std::vector<size_t> changes;
		std::vector<size_t> activations;
		std::vector<size_t> accepts;
		std::vector<MyGUI::IBNotifyItemData> notifications;
		// Destroy the GUI before the callback state it references.
		unittest::TestContext context;
		MyGUI::ItemBox* box;

		ItemBoxFixture()
		{
			unittest::createInputLayer();
			unittest::loadResources("UnitTest_ItemBox/TestSkin.xml");
			box = context.getGui().createWidget<MyGUI::ItemBox>(
				"TestItemBox",
				MyGUI::IntCoord(50, 50, 120, 80),
				MyGUI::Align::Default,
				"Main");
			box->requestCoordItem = MyGUI::newDelegate(this, &ItemBoxFixture::coordinate);
			box->requestCreateWidgetItem = MyGUI::newDelegate(this, &ItemBoxFixture::create);
			box->requestDrawItem = MyGUI::newDelegate(this, &ItemBoxFixture::draw);
			box->eventChangeItemPosition += MyGUI::newDelegate(this, &ItemBoxFixture::change);
			box->eventMouseItemActivate += MyGUI::newDelegate(this, &ItemBoxFixture::activate);
			box->eventSelectItemAccept += MyGUI::newDelegate(this, &ItemBoxFixture::accept);
			box->eventNotifyItem += MyGUI::newDelegate(this, &ItemBoxFixture::notify);
			// Remeasure after installing the item-size callback: three columns and four rows.
			box->setSize(120, 80);
		}

		void coordinate(MyGUI::ItemBox*, MyGUI::IntCoord& _coord, bool)
		{
			_coord = MyGUI::IntCoord(0, 0, 40, 20);
		}

		void create(MyGUI::ItemBox*, MyGUI::Widget* _item)
		{
			created.push_back(_item);
			_item->setNeedKeyFocus(true);
		}

		void draw(MyGUI::ItemBox* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _info)
		{
			require(_info.index < _sender->getItemCount(), "Draw callbacks must reference an existing item");
			require(!_info.drag, "Ordinary redraws must not be marked as drag previews");
			draws.push_back(_info);
			_item->setUserData(_info.index);
		}

		void change(MyGUI::ItemBox*, size_t _index)
		{
			changes.push_back(_index);
		}

		void activate(MyGUI::ItemBox*, size_t _index)
		{
			activations.push_back(_index);
		}

		void accept(MyGUI::ItemBox*, size_t _index)
		{
			accepts.push_back(_index);
		}

		void notify(MyGUI::ItemBox*, const MyGUI::IBNotifyItemData& _info)
		{
			notifications.push_back(_info);
		}

		void populate(int _count)
		{
			for (int index = 0; index < _count; ++index)
				box->addItem(index, index == _count - 1);
		}

		MyGUI::Widget* item(size_t _index)
		{
			auto* widget = box->getWidgetByIndex(_index);
			require(widget != nullptr, "The requested item must have a visible widget");
			require(box->getIndexByWidget(widget) == _index, "Widget lookup must round-trip to the logical item index");
			require(
				*widget->getUserData<size_t>() == _index,
				"Reused widgets must be redrawn with their current index");
			return widget;
		}
	};

	void testItemData()
	{
		ItemBoxFixture fixture;
		auto* box = fixture.box;
		require(box->getItemCount() == 0 && box->getIndexSelected() == MyGUI::ITEM_NONE, "New boxes must be empty");
		require(box->getWidgetByIndex(0) == nullptr, "Empty boxes must have no item widgets");
		box->removeAllItems();
		box->addItem(10);
		box->insertItemAt(0, 20);
		box->insertItemAt(box->getItemCount(), 30);
		box->insertItemAt(MyGUI::ITEM_NONE, 40);
		require(box->getItemCount() == 4, "Both explicit end and ITEM_NONE insertions must append");
		const std::vector<int> expected{20, 10, 30, 40};
		for (size_t index = 0; index < expected.size(); ++index)
			require(*box->getItemDataAt<int>(index) == expected[index], "Insertion must preserve item data order");
		box->setItemDataAt(1, std::string("replacement"));
		require(*box->getItemDataAt<std::string>(1) == "replacement", "Replacing data must support a new type");
		require(box->getItemDataAt<int>(1, false) == nullptr, "Nonthrowing casts must reject the wrong data type");
		box->clearItemDataAt(1);
		require(box->getItemDataAt<std::string>(1, false) == nullptr, "Clearing data must remove the stored value");
		box->removeItemAt(1);
		require(box->getItemCount() == 3 && *box->getItemDataAt<int>(1) == 30, "Removal must shift remaining data");
		box->removeAllItems();
		for (auto* widget : fixture.created)
			require(!widget->getVisible(), "Clearing items must hide all pooled widgets");
		box->addItem();
		require(
			box->getItemCount() == 1 && box->getItemDataAt<int>(0, false) == nullptr,
			"Empty data must work after reuse");
		fixture.item(0);
	}

	void testSelectionMutations()
	{
		ItemBoxFixture fixture;
		auto* box = fixture.box;
		fixture.populate(3);
		box->setIndexSelected(1);
		box->insertItemAt(0, 10);
		require(
			box->getIndexSelected() == 2 && *box->getItemDataAt<int>(2) == 1,
			"Insertion before selection must preserve identity");
		box->insertItemAt(3, 11);
		require(box->getIndexSelected() == 2, "Insertion after selection must preserve its index");
		box->removeItemAt(0);
		require(box->getIndexSelected() == 1, "Removal before selection must shift its index");
		box->removeItemAt(3);
		require(box->getIndexSelected() == 1, "Removal after selection must preserve its index");
		box->removeItemAt(1);
		require(
			box->getIndexSelected() == 1 && *box->getItemDataAt<int>(1) == 11,
			"Removing a selected middle item must select its successor");
		box->removeItemAt(1);
		require(box->getIndexSelected() == 0, "Removing the selected last item must select its predecessor");
		box->removeItemAt(0);
		require(box->getIndexSelected() == MyGUI::ITEM_NONE, "Removing the only item must clear selection");
		fixture.populate(3);
		box->setIndexSelected(1);
		box->clearIndexSelected();
		require(
			box->getIndexSelected() == MyGUI::ITEM_NONE && box->getItemCount() == 3,
			"Clearing selection must preserve items");
		box->setIndexSelected(2);
		box->removeAllItems();
		require(box->getIndexSelected() == MyGUI::ITEM_NONE, "Clearing items must clear selection");
		require(fixture.changes.empty(), "Programmatic selection and mutations must not emit user selection events");
	}

	void testRedraws()
	{
		ItemBoxFixture fixture;
		auto* box = fixture.box;
		fixture.populate(3);
		fixture.draws.clear();
		box->setIndexSelected(1);
		require(
			fixture.draws.size() == 1 && fixture.draws[0].index == 1 && fixture.draws[0].select &&
				!fixture.draws[0].update,
			"Selecting an item must request a state-only redraw");
		box->setIndexSelected(1);
		require(fixture.draws.size() == 1, "Selecting the same item must not redraw it again");
		fixture.draws.clear();
		box->setIndexSelected(2);
		require(
			fixture.draws.size() == 2 && fixture.draws[0].index == 1 && !fixture.draws[0].select &&
				fixture.draws[1].index == 2 && fixture.draws[1].select,
			"Changing selection must redraw both old and new items");
		fixture.draws.clear();
		box->setItemDataAt(2, 99);
		require(
			fixture.draws.size() == 1 && fixture.draws[0].index == 2 && fixture.draws[0].update &&
				fixture.draws[0].select,
			"Replacing visible data must request a full redraw preserving selection");
		fixture.draws.clear();
		box->redrawItemAt(0);
		require(
			fixture.draws.size() == 1 && fixture.draws[0].index == 0 && fixture.draws[0].update,
			"Explicit item redraw must refresh only that item");
		fixture.draws.clear();
		box->redrawAllItems();
		require(fixture.draws.size() == 3, "Redrawing a small box must visit every item");
		for (size_t index = 0; index < 3; ++index)
			require(
				fixture.draws[index].index == index && fixture.draws[index].update,
				"Full redraw must refresh item contents");
	}

	void testDeferredUpdates()
	{
		ItemBoxFixture fixture;
		auto* box = fixture.box;
		box->addItem(10, false);
		box->insertItemAt(0, 20, false);
		require(
			box->getItemCount() == 2 && fixture.created.empty() && fixture.draws.empty(),
			"Deferred insertion must update data without creating widgets");
		box->addItem(30);
		for (size_t index = 0; index < 3; ++index)
			fixture.item(index);
		fixture.draws.clear();
		box->removeItemAt(0, false);
		require(
			box->getItemCount() == 2 && *box->getItemDataAt<int>(0) == 10 && fixture.draws.empty(),
			"Deferred removal must update data without redrawing");
		box->removeItemAt(1);
		require(
			box->getItemCount() == 1 && box->getWidgetByIndex(1) == nullptr,
			"The next immediate mutation must refresh deferred changes");
		fixture.item(0);
	}

	template<bool Vertical>
	void testGridAndScrolling()
	{
		ItemBoxFixture fixture;
		auto* box = fixture.box;
		box->setVerticalAlignment(Vertical);
		require(box->getVerticalAlignment() == Vertical, "Grid orientation must round-trip");
		fixture.populate(120);
		const size_t perLine = Vertical ? 3 : 4;
		const auto offset = [](int value)
		{
			return Vertical ? MyGUI::IntPoint(0, value) : MyGUI::IntPoint(value, 0);
		};
		require(box->getViewSize() == MyGUI::IntSize(120, 80), "The client must fill the test box");
		require(
			fixture.item(0)->getCoord() == MyGUI::IntCoord(0, 0, 40, 20),
			"The first cell must use the requested size");
		require(
			fixture.item(1)->getPosition() == (Vertical ? MyGUI::IntPoint(40, 0) : MyGUI::IntPoint(0, 20)),
			"Cells must follow grid orientation");
		require(
			fixture.item(perLine)->getPosition() == (Vertical ? MyGUI::IntPoint(0, 20) : MyGUI::IntPoint(40, 0)),
			"Cells must wrap at the viewport boundary");
		require(
			fixture.created.size() < box->getItemCount(),
			"Large boxes must create widgets only for a window of items");
		require(box->getWidgetByIndex(119) == nullptr, "Distant items must have no visible widget");
		auto* firstWidget = fixture.item(0);
		const size_t poolSize = fixture.created.size();
		box->setIndexSelected(perLine * 2);
		box->setViewOffset(offset(Vertical ? 45 : 85));
		require(
			box->getViewOffset() == offset(Vertical ? 45 : 85),
			"Partial-cell scrolling must preserve the requested offset");
		require(fixture.item(perLine * 2) == firstWidget, "Scrolling must reuse widgets for new logical items");
		require(
			firstWidget->getPosition() == (Vertical ? MyGUI::IntPoint(0, -5) : MyGUI::IntPoint(-5, 0)),
			"The first cell must retain the partial scroll offset");
		require(
			box->getWidgetByIndex(0) == nullptr && fixture.created.size() == poolSize,
			"Scrolling must recycle the existing widget pool");
		require(box->getIndexSelected() == perLine * 2, "Scrolling must preserve logical selection");
		fixture.draws.clear();
		box->redrawItemAt(0);
		require(fixture.draws.empty(), "Redrawing an item before the visible window must not redraw another item");
		box->setViewOffset(offset(10000));
		require(
			box->getViewOffset() == offset(Vertical ? 720 : 1080),
			"Scroll offsets must clamp to the last content page");
		fixture.item(119);
		box->setViewOffset(offset(-10));
		require(box->getViewOffset() == MyGUI::IntPoint(), "Negative scroll offsets must clamp to the beginning");
		fixture.item(0);
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(60, 60, 0);
		input.injectMouseMove(60, 60, -1);
		require(
			box->getViewOffset() == offset(Vertical ? 20 : 40),
			"Mouse wheel input over an item must scroll one row or column");
		fixture.item(perLine);
		input.injectMouseMove(60, 60, 0);
		require(box->getViewOffset() == MyGUI::IntPoint(), "Reverse wheel input must return to the first line");
		input.injectMouseMove(0, 0, 0);
		box->setViewOffset(offset(100));
		box->removeAllItems();
		require(box->getViewOffset() == MyGUI::IntPoint(), "Clearing scrolled content must reset the offset");
		fixture.populate(8);
		box->setSize(80, 40);
		require(
			fixture.item(2)->getPosition() == (Vertical ? MyGUI::IntPoint(0, 20) : MyGUI::IntPoint(40, 0)),
			"Resizing must recompute grid wrapping");
		box->setViewOffset(offset(100));
		box->setCoord(50, 50, 400, 200);
		require(box->getViewOffset() == MyGUI::IntPoint(), "Enlarging the viewport to fit must reset scrolling");
		fixture.item(7);
	}

	void click(MyGUI::Widget* _widget, MyGUI::MouseButton _button = MyGUI::MouseButton::Left)
	{
		const auto coord = _widget->getAbsoluteCoord();
		const int x = coord.left + coord.width / 2;
		const int y = coord.top + coord.height / 2;
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(x, y, 0);
		input.injectMousePress(x, y, _button);
		input.injectMouseRelease(x, y, _button);
	}

	void testInputEvents()
	{
		ItemBoxFixture fixture;
		fixture.populate(3);
		auto* box = fixture.box;
		click(fixture.item(1));
		require(
			box->getIndexSelected() == 1 && fixture.changes == std::vector<size_t>{1} &&
				fixture.activations == std::vector<size_t>{1},
			"Clicking an item must select and activate its logical index");
		require(
			fixture.notifications.size() == 2 &&
				fixture.notifications[0].notify == MyGUI::IBNotifyItemData::MousePressed &&
				fixture.notifications[1].notify == MyGUI::IBNotifyItemData::MouseReleased,
			"Clicks must forward press and release notifications");
		for (const auto& info : fixture.notifications)
			require(
				info.index == 1 && info.id == MyGUI::MouseButton::Left && info.x == 110 && info.y == 60,
				"Mouse notifications must retain item index, button, and absolute coordinates");
		click(fixture.item(1));
		require(
			fixture.changes.size() == 1 && fixture.activations == std::vector<size_t>({1, 1}),
			"Repeated clicks must activate without repeating selection changes");
		require(fixture.accepts == std::vector<size_t>{1}, "Double-clicking must accept the clicked item");
		click(fixture.item(2), MyGUI::MouseButton::Right);
		require(
			box->getIndexSelected() == 1 && fixture.activations.size() == 2,
			"Right clicks must preserve selection and activation");
		fixture.notifications.clear();
		MyGUI::InputManager::getInstance().setKeyFocusWidget(fixture.item(1));
		unittest::keyStroke(MyGUI::KeyCode::A, 'a');
		require(
			fixture.notifications.size() == 2 &&
				fixture.notifications[0].notify == MyGUI::IBNotifyItemData::KeyPressed &&
				fixture.notifications[0].index == 1 && fixture.notifications[0].code == MyGUI::KeyCode::A &&
				fixture.notifications[0].key == 'a' &&
				fixture.notifications[1].notify == MyGUI::IBNotifyItemData::KeyReleased &&
				fixture.notifications[1].index == 1 && fixture.notifications[1].code == MyGUI::KeyCode::A,
			"Keyboard notifications must retain focused item, key code, and text");
		click(box->getClientWidget());
		require(
			box->getIndexSelected() == MyGUI::ITEM_NONE &&
				fixture.changes == std::vector<size_t>({1, MyGUI::ITEM_NONE}) &&
				fixture.activations.back() == MyGUI::ITEM_NONE,
			"Clicking empty client space must clear selection and report ITEM_NONE");
	}

}

int main()
{
	return unittest::runTests({
		{"Item data and reuse", testItemData},
		{"Selection mutations", testSelectionMutations},
		{"Draw callback state", testRedraws},
		{"Deferred item updates", testDeferredUpdates},
		{"Vertical grid and scrolling", testGridAndScrolling<true>},
		{"Horizontal grid and scrolling", testGridAndScrolling<false>},
		{"Mouse and keyboard notifications", testInputEvents},
	});
}
