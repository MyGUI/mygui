#ifndef UNITTEST_LIST_ITEM_HEIGHT_H_
#define UNITTEST_LIST_ITEM_HEIGHT_H_

#include "MyGUI.h"
#include "ListTestChecks.h"

namespace unittest
{

	class HeightTestListBox : public MyGUI::ListBox
	{
		MYGUI_RTTI_DERIVED(HeightTestListBox)

	public:
		MyGUI::ScrollBar* getScrollBar()
		{
			MyGUI::ScrollBar* scroll = nullptr;
			assignWidget(scroll, "VScroll");
			return scroll;
		}
	};

	inline void checkItemHeightLayout(MyGUI::ListBox* _list, int _height, int _position)
	{
		checkEqual(_list->getItemHeight(), _height, "Item height");
		const auto* client = _list->getClientWidget() == nullptr ? _list : _list->getClientWidget();
		checkEqual(
			_list->getOptimalHeight(),
			_list->getHeight() - client->getHeight() + (int)_list->getItemCount() * _height,
			"Optimal height");
		for (size_t index = 0; index < _list->getItemCount(); ++index)
		{
			const int top = (int)index * _height - _position;
			if (top + _height <= 0 || top >= client->getHeight())
				continue;
			auto* row = _list->getWidgetByIndex(index);
			require(row != nullptr && row->getVisible(), "Every item in the viewport must have a visible row");
			checkEqual(row->getCoord(), MyGUI::IntCoord(0, top, client->getWidth(), _height), "Row coordinates");
			if (row->getSubWidgetText() != nullptr)
				checkEqual(row->castType<MyGUI::Button>()->getCaption(), _list->getItemNameAt(index), "Row caption");
			checkEqual(
				row->castType<MyGUI::Button>()->getStateSelected(),
				index == _list->getIndexSelected(),
				"Row selection");
		}
		_list->_checkAlign();
	}

	inline void testListItemHeight()
	{
		loadResources("UnitTest_MultiList/TestSkin.xml");
		MyGUI::FactoryManager::getInstance().registerFactory<HeightTestListBox>(
			MyGUI::WidgetManager::getInstance().getCategoryName());
		auto& gui = MyGUI::Gui::getInstance();
		auto* list = gui.createWidget<HeightTestListBox>(
			"TestHeightListBox",
			MyGUI::IntCoord(0, 0, 120, 104),
			MyGUI::Align::Default,
			"Main");
		auto* scroll = list->getScrollBar();
		require(scroll != nullptr, "The test skin must have a scrollbar");
		checkItemHeightLayout(list, 20, 0);
		list->setItemHeight(25);
		checkItemHeightLayout(list, 25, 0);
		for (size_t index = 0; index < 30; ++index)
			list->addItem(MyGUI::utility::toString("Item ", index), index);
		list->setIndexSelected(8);
		list->setScrollPosition(83);
		checkItemHeightLayout(list, 25, 83);

		// Shrinking must allocate enough rows; growing must hide surplus rows.
		for (const int height : {10, 40, 40, 15, 150, 12})
		{
			list->setItemHeight(height);
			checkItemHeightLayout(list, height, 83);
			checkEqual(scroll->getScrollPosition(), size_t(83), "Preserved pixel scroll position");
			checkEqual(scroll->getScrollPage(), (size_t)height, "Scroll step");
			checkEqual(scroll->getScrollRange(), size_t(30 * height - 100 + 1), "Scroll range");
			checkEqual(list->getIndexSelected(), size_t(8), "Preserved selection");
			checkEqual(*list->getItemDataAt<size_t>(8), size_t(8), "Preserved item data");
		}

		list->getWidgetByIndex(8)->eventMouseWheel(list->getWidgetByIndex(8), -1);
		checkItemHeightLayout(list, 12, 95);
		checkEqual(scroll->getScrollPosition(), size_t(95), "Wheel uses updated height");

		// Clamp the old scroll position to the new bottom edge, including a partial row.
		list->beginToItemLast();
		list->setItemHeight(7);
		checkItemHeightLayout(list, 7, 110);
		checkEqual(scroll->getScrollPosition(), size_t(110), "Clamped scroll position");
		list->setProperty("ItemHeight", "3");
		checkItemHeightLayout(list, 3, 0);
		require(!scroll->getVisible(), "The scrollbar must hide when all items fit");
		checkEqual(scroll->getScrollPosition(), size_t(0), "Scroll resets when all items fit");
		list->setItemHeight(20);
		checkItemHeightLayout(list, 20, 0);
		require(scroll->getVisible(), "The scrollbar must reappear when items overflow");

		list->setItemHeight(0);
		checkItemHeightLayout(list, 1, 0);
		list->setItemHeight(-10);
		checkItemHeightLayout(list, 1, 0);
		list->removeAllItems();
		list->setItemHeight(30);
		list->addItem("Reused");
		checkItemHeightLayout(list, 30, 0);
		gui.destroyWidget(list);

		// Skins without a scrollbar must also resize existing rows and create new ones.
		list = gui.createWidget<HeightTestListBox>(
			"Default",
			MyGUI::IntCoord(0, 0, 120, 100),
			MyGUI::Align::Default,
			"Main");
		list->setItemHeight(30);
		for (int index = 0; index < 20; ++index)
			list->addItem(MyGUI::utility::toString(index));
		checkItemHeightLayout(list, 30, 0);
		list->setItemHeight(5);
		checkItemHeightLayout(list, 5, 0);
		gui.destroyWidget(list);
	}

}

#endif // UNITTEST_LIST_ITEM_HEIGHT_H_
