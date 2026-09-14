#ifndef UNITTEST_MULTILIST_ITEM_HEIGHT_H_
#define UNITTEST_MULTILIST_ITEM_HEIGHT_H_

#include "UnitTest_ListItemHeight.h"

namespace unittest
{

	inline MyGUI::ListBox* getColumnList(MyGUI::MultiListBox* _list, size_t _column)
	{
		return _list->_getItemAt(_column)->getChildAt(0)->castType<MyGUI::ListBox>();
	}

	inline void checkMultiListHeight(MyGUI::MultiListBox* _list, int _height, int _position)
	{
		checkEqual(_list->getItemHeight(), _height, "MultiListBox item height");
		for (size_t column = 0; column < _list->getColumnCount(); ++column)
		{
			auto* list = getColumnList(_list, column);
			checkItemHeightLayout(list, _height, _position);
			checkEqual(list->_getScrollPosition(), (size_t)_position, "Synchronized scroll position");
			checkEqual(_list->_getItemAt(column)->getTop(), 20, "Header height must not change");
		}
	}

	inline void testMultiListItemHeight()
	{
		auto& gui = MyGUI::Gui::getInstance();
		auto* list = gui.createWidget<MyGUI::MultiListBox>(
			"TestHeightMultiListBox",
			MyGUI::IntCoord(0, 0, 240, 124),
			MyGUI::Align::Default,
			"Main");
		checkEqual(list->getItemHeight(), 1, "Empty list default height");
		list->addColumn("First", 120);
		checkMultiListHeight(list, 20, 0);
		list->removeAllColumns();
		list->setItemHeight(25);
		checkEqual(list->getItemHeight(), 25, "Explicit height before columns exist");
		list->addColumn("First", 120);
		list->addColumn("Second", 120);
		for (size_t index = 0; index < 30; ++index)
		{
			list->addItem(MyGUI::utility::toString(index), index);
			list->setSubItemNameAt(1, index, MyGUI::utility::toString("Cell ", index));
		}
		// Finish any pending sorting before checking that resizing preserves logical rows.
		gui.eventFrameStart(0.016f);
		list->setIndexSelected(8);
		const auto selectedName = list->getItemNameAt(8);
		const auto selectedData = *list->getItemDataAt<size_t>(8);
		auto* first = getColumnList(list, 0);
		first->setScrollPosition(83);
		first->_sendEventChangeScroll(83);
		checkMultiListHeight(list, 25, 83);
		for (const int height : {10, 40, 40, 12})
		{
			list->setItemHeight(height);
			checkMultiListHeight(list, height, 83);
			checkEqual(list->getIndexSelected(), size_t(8), "Preserved multi-list selection");
			checkEqual(list->getItemNameAt(8), selectedName, "Preserved sort order");
			checkEqual(*list->getItemDataAt<size_t>(8), selectedData, "Preserved multi-list data");
		}
		list->addColumn("Later", 120);
		checkMultiListHeight(list, 12, 83);
		first->beginToItemLast();
		first->_sendEventChangeScroll(first->_getScrollPosition());
		list->setItemHeight(7);
		checkMultiListHeight(list, 7, 110);
		list->setProperty("ItemHeight", "3");
		checkMultiListHeight(list, 3, 0);
		list->setProperty("ItemHeight", "-9");
		checkMultiListHeight(list, 1, 0);
		list->setItemHeight(0);
		checkMultiListHeight(list, 1, 0);
		list->removeAllColumns();
		list->setProperty("ItemHeight", "32");
		list->addColumn("Recreated", 120);
		list->addItem("One");
		checkMultiListHeight(list, 32, 0);
		list->removeAllColumns();
		list->addColumn("Retained", 120);
		list->addItem("Two");
		checkMultiListHeight(list, 32, 0);
		gui.destroyWidget(list);
	}

	inline void testListItemHeightLayout()
	{
		auto* resource =
			MyGUI::ResourceManager::getInstance().getByName("TestListHeightLayout")->castType<MyGUI::ResourceLayout>();
		auto widgets = resource->createLayout();
		auto* list = widgets.at(0)->castType<MyGUI::ListBox>();
		checkItemHeightLayout(list, 32, 0);
		auto* multiList = widgets.at(1)->castType<MyGUI::MultiListBox>();
		checkMultiListHeight(multiList, 32, 0);
		MyGUI::LayoutManager::getInstance().unloadLayout(widgets);
	}

}

#endif // UNITTEST_MULTILIST_ITEM_HEIGHT_H_
