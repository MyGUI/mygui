#include "BehaviourTestSupport.h"
#include <algorithm>
#include <array>
#include <vector>

namespace
{

	using unittest::require;

	template<typename List>
	List* createList(MyGUI::Gui& _gui)
	{
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
		return _gui.createWidget<List>("Default", MyGUI::IntCoord(0, 0, 400, 200), MyGUI::Align::Default, "Main");
	}

	template<typename List>
	void checkSelectionMutations(List* _list)
	{
		_list->addItem("first", 10);
		_list->addItem("selected", 20);
		_list->addItem("last", 30);
		_list->setIndexSelected(1);
		_list->insertItemAt(0, "inserted", 40);
		require(_list->getIndexSelected() == 2, "Inserting before selection must preserve the selected item identity");
		require(
			*_list->template getItemDataAt<int>(_list->getIndexSelected()) == 20,
			"Selection must retain the original item data");
		_list->removeItemAt(0);
		require(_list->getIndexSelected() == 1, "Removing before selection must adjust the selected index");
		_list->removeItemAt(2);
		require(_list->getIndexSelected() == 1, "Removing after selection must preserve its index");
		_list->removeItemAt(1);
		// Libraries differ on selecting a neighbour versus clearing selection after deletion.
		// Either is valid here, but a dangling selected index is not.
		require(
			_list->getIndexSelected() == MyGUI::ITEM_NONE || _list->getIndexSelected() < _list->getItemCount(),
			"Removing the selected item must leave selection empty or pointing at a surviving item");
		_list->removeAllItems();
		require(_list->getIndexSelected() == MyGUI::ITEM_NONE, "Clearing all items must clear selection");
		_list->addItem("reused", 50);
		require(
			_list->getItemCount() == 1 && *_list->template getItemDataAt<int>(0) == 50,
			"Cleared lists must be reusable");
		_list->setIndexSelected(0);
		_list->clearIndexSelected();
		require(_list->getIndexSelected() == MyGUI::ITEM_NONE, "Explicitly clearing selection must preserve items");
	}

	void testListSelection()
	{
		unittest::TestContext context;
		checkSelectionMutations(createList<MyGUI::ListBox>(context.getGui()));
	}

	void testMultiListSelection()
	{
		unittest::TestContext context;
		auto* list = createList<MyGUI::MultiListBox>(context.getGui());
		list->addColumn("name", 100);
		checkSelectionMutations(list);
	}

	void testColumnMutations()
	{
		unittest::TestContext context;
		auto* list = createList<MyGUI::MultiListBox>(context.getGui());
		list->addColumn("name", 100, 10);
		list->addColumn("value", 100, 20);
		list->addItem("beta", 1);
		list->addItem("alpha", 2);
		list->setSubItemNameAt(1, 0, "B");
		list->setSubItemNameAt(1, 1, "A");
		list->setSubItemDataAt(1, 0, 101);
		list->setSubItemDataAt(1, 1, 102);
		list->insertColumnAt(1, "inserted", 80, 30);
		require(
			list->getColumnCount() == 3 && list->getItemCount() == 2,
			"Inserting a column must preserve populated rows");
		require(
			list->getColumnNameAt(2) == "value" && *list->getColumnDataAt<int>(2) == 20,
			"Existing column metadata must move with its column");
		for (size_t row = 0; row < 2; ++row)
		{
			require(
				list->getSubItemNameAt(1, row).empty() && list->getSubItemDataAt<int>(1, row, false) == nullptr,
				"A new column must contain empty cells for existing rows");
			require(
				*list->getSubItemDataAt<int>(2, row) == 101 + static_cast<int>(row),
				"Inserting columns must preserve existing cell data");
		}
		list->sortByColumn(2);
		list->removeColumnAt(2);
		require(
			list->getItemCount() == 2 && list->getItemNameAt(0) == "beta" && list->getItemNameAt(1) == "alpha",
			"Removing the sorted column must preserve logical row identities");
		list->sortByColumn(0);
		list->removeColumnAt(1);
		list->addColumn("replacement", 90);
		list->setSubItemNameAt(1, 0, "new");
		require(
			list->getSubItemNameAt(1, 0) == "new" && list->getSubItemNameAt(1, 1).empty(),
			"Removed columns must not leak cell values into replacements");
		list->removeAllColumns();
		require(list->getColumnCount() == 0, "Removing all columns must empty the column collection");
		list->addColumn("fresh", 100);
		list->addItem("fresh row", 99);
		require(
			list->getItemCount() == 1 && *list->getItemDataAt<int>(0) == 99,
			"A list must be reusable after removing all columns");
	}

	void collectColumnLists(MyGUI::Widget* _widget, std::vector<MyGUI::ListBox*>& _lists)
	{
		for (auto* child : _widget->getChildWidgets())
		{
			if (auto* list = child->castType<MyGUI::ListBox>(false))
				_lists.push_back(list);
			else
				collectColumnLists(child, _lists);
		}
	}

	struct NumericOrder
	{
		int calls{0};
		void compare(MyGUI::MultiListBox* _sender, size_t _column, size_t _left, size_t _right, bool& _less)
		{
			++calls;
			_less = std::stoi(_sender->getSubItemNameAt(_column, _left).asUTF8()) <
				std::stoi(_sender->getSubItemNameAt(_column, _right).asUTF8());
		}
	};

	void testDisplayedRows()
	{
		NumericOrder numeric;
		unittest::TestContext context;
		auto* list = createList<MyGUI::MultiListBox>(context.getGui());
		list->addColumn("number", 100);
		list->addColumn("identity", 100);
		const std::array<const char*, 3> names = {"10", "2", "30"};
		for (size_t row = 0; row < names.size(); ++row)
		{
			list->addItem(names[row], row);
			list->setSubItemNameAt(1, row, MyGUI::utility::toString("row-", row));
			list->setSubItemDataAt(1, row, row);
		}
		list->setIndexSelected(1);
		std::vector<MyGUI::ListBox*> columns;
		// Inspect displayed cells through public widget traversal; logical MultiList indices intentionally do not change on sorting.
		collectColumnLists(list, columns);
		require(columns.size() == 2, "Each column must expose a displayed list");
		std::sort(
			columns.begin(),
			columns.end(),
			[](auto* left, auto* right) { return left->getAbsoluteLeft() < right->getAbsoluteLeft(); });
		auto checkOrder = [&](const std::array<size_t, 3>& order)
		{
			for (size_t display = 0; display < order.size(); ++display)
			{
				const size_t row = order[display];
				require(
					columns[0]->getItemNameAt(display) == names[row],
					"The sorted column must follow the requested order");
				require(
					columns[1]->getItemNameAt(display) == MyGUI::utility::toString("row-", row),
					"Other columns must display the same logical row");
				for (auto* column : columns)
					require(
						*column->getItemDataAt<size_t>(display) == row,
						"Displayed cell data must follow its row during sorting");
			}
			require(list->getIndexSelected() == 1, "Sorting must preserve logical selection");
			for (size_t row = 0; row < names.size(); ++row)
				require(list->getItemNameAt(row) == names[row], "Sorting must preserve documented logical indices");
		};
		list->sortByColumn(0);
		checkOrder({0, 1, 2});
		list->sortByColumn(0, true);
		checkOrder({2, 1, 0});
		list->requestOperatorLess = MyGUI::newDelegate(&numeric, &NumericOrder::compare);
		list->sortByColumn(0);
		require(numeric.calls > 0, "Sorting must invoke the custom comparator");
		checkOrder({1, 0, 2});
		list->sortByColumn(0, true);
		checkOrder({2, 0, 1});
	}

}

int main()
{
	return unittest::runTests({
		{"List selection mutations", testListSelection},
		{"MultiList selection mutations", testMultiListSelection},
		{"Populated column mutations", testColumnMutations},
		{"Displayed row identity and custom sorting", testDisplayedRows},
	});
}
