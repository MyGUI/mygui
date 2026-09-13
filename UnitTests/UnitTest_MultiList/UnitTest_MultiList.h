/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef UNITTEST_MULTILIST_H_
#define UNITTEST_MULTILIST_H_

#include "MyGUI.h"
#include "ListTestChecks.h"
#include <random>
#include <algorithm>
#include "Mirror_MultiList.h"

namespace unittest
{

	class UnitTest_MultiList
	{
	private:
		MyGUI::MultiListBox* original_list;
		Mirror_MultiList mirror_list;
		std::mt19937& mRandom;
		std::string mOperation{"initial state"};
		size_t count_items;
		size_t count_columns;

	public:
		explicit UnitTest_MultiList(std::mt19937& _random) :
			mRandom(_random)
		{
			original_list = MyGUI::Gui::getInstance().createWidget<MyGUI::MultiListBox>(
				"Default",
				MyGUI::IntCoord(300, 100, 400, 400),
				MyGUI::Align::Default,
				"Main");
			count_items = 0;
			count_columns = 0;

			original_list->addColumn("1");
			original_list->setColumnResizingPolicyAt(0, MyGUI::ResizingPolicy::Fill);
			mirror_list.addColumn("1");
			count_columns = 1;

			original_list->addColumn("2");
			original_list->setColumnResizingPolicyAt(1, MyGUI::ResizingPolicy::Fill);
			mirror_list.addColumn("2");
			count_columns = 2;

			original_list->addColumn("3");
			original_list->setColumnResizingPolicyAt(2, MyGUI::ResizingPolicy::Fill);
			mirror_list.addColumn("3");
			count_columns = 3;
		}

		~UnitTest_MultiList()
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(original_list);
		}

		void checkMultiList()
		{
			checkEqual(original_list->getItemCount(), count_items, mOperation + ": MultiListBox item count");
			checkEqual(original_list->getColumnCount(), count_columns, mOperation + ": MultiListBox column count");
			checkEqual(mirror_list.getItemCount(), count_items, mOperation + ": reference item count");
			checkEqual(mirror_list.getColumnCount(), count_columns, mOperation + ": reference column count");
			for (size_t item = 0; item < count_items; ++item)
			{
				const auto row = MyGUI::utility::toString(mOperation, ": MultiListBox item ", item);
				checkEqual(original_list->getItemNameAt(item), mirror_list.getItemNameAt(item), row + " name");
				checkData(
					original_list->getItemDataAt<size_t>(item, false),
					mirror_list.getItemDataAt<size_t>(item, false),
					row + " data");
				for (size_t column = 0; column < count_columns; ++column)
				{
					const auto cell = MyGUI::utility::toString(row, ", column ", column);
					checkEqual(
						original_list->getSubItemNameAt(column, item),
						mirror_list.getSubItemNameAt(column, item),
						cell + " name");
					checkData(
						original_list->getSubItemDataAt<size_t>(column, item, false),
						mirror_list.getSubItemDataAt<size_t>(column, item, false),
						cell + " data");
				}
			}
		}

		void Begin()
		{
			mOperation = "Begin";
			if (count_columns == 0)
				return;
			size_t count = original_list->getItemCount();
			if (count == 0)
				return;

			size_t index = mRandom() % count;
			original_list->setIndexSelected(index);

			checkMultiList();
		}

		void Begin(size_t _count)
		{
			while (_count > 0)
			{
				Begin();
				--_count;
			}
		}

		void AddItem()
		{
			mOperation = "AddItem";
			if (count_columns == 0)
				return;

			size_t item = mRandom();
			mirror_list.addItem(MyGUI::utility::toString(item), item);
			original_list->addItem(MyGUI::utility::toString(item), item);
			count_items++;

			checkMultiList();
		}

		void AddItem(size_t _count)
		{
			while (_count > 0)
			{
				AddItem();
				--_count;
			}
		}

		void InsertItem()
		{
			mOperation = "InsertItem";
			if (count_columns == 0)
				return;

			size_t index = count_items == 0 ? 0 : (mRandom() % count_items);
			size_t item = mRandom();

			mirror_list.insertItemAt(index, MyGUI::utility::toString(item), item);
			original_list->insertItemAt(index, MyGUI::utility::toString(item), item);

			count_items++;

			checkMultiList();
		}

		void InsertItem(size_t _count)
		{
			while (_count > 0)
			{
				InsertItem();
				--_count;
			}
		}

		void RemoveItem()
		{
			mOperation = "RemoveItem";
			if (count_columns == 0)
				return;
			if (count_items == 0)
				return;

			size_t index = mRandom() % count_items;

			mirror_list.removeItemAt(index);
			original_list->removeItemAt(index);

			count_items--;

			checkMultiList();
		}

		void RemoveItem(size_t _count)
		{
			while (_count > 0)
			{
				RemoveItem();
				--_count;
			}
		}

		void SwapItems()
		{
			mOperation = "SwapItems";
			if (count_columns == 0)
				return;
			if (count_items == 0)
				return;

			size_t index1 = mRandom() % count_items;
			size_t index2 = mRandom() % count_items;

			mirror_list.swapItemsAt(index1, index2);
			original_list->swapItemsAt(index1, index2);

			checkMultiList();
		}

		void SwapItems(size_t _count)
		{
			while (_count > 0)
			{
				SwapItems();
				--_count;
			}
		}

		void sortColumn(size_t _column, bool _descending)
		{
			mOperation =
				MyGUI::utility::toString("SortItems column ", _column, _descending ? " descending" : " ascending");
			std::vector<MyGUI::UString> expected;
			for (size_t item = 0; item < count_items; ++item)
				expected.push_back(mirror_list.getSubItemNameAt(_column, item));
			std::sort(expected.begin(), expected.end());
			if (_descending)
				std::reverse(expected.begin(), expected.end());

			const auto selected = original_list->getIndexSelected();
			original_list->sortByColumn(_column);
			// The backward argument reverses the current direction, rather than setting it.
			if (_descending)
				original_list->sortByColumn(_column, true);
			checkEqual(original_list->getIndexSelected(), selected, mOperation + ": selected logical item");
			checkMultiList(); // Sorting must preserve logical row indices and data in every column.

			// Public MultiListBox indices stay logical; inspect the column ListBox for display order.
			MyGUI::ListBox* displayed = nullptr;
			for (auto* child : original_list->_getItemAt(_column)->getChildWidgets())
			{
				if (auto* list = child->castType<MyGUI::ListBox>(false))
				{
					require(displayed == nullptr, mOperation + ": multiple column lists");
					displayed = list;
				}
			}
			require(displayed != nullptr, mOperation + ": missing column list");
			checkEqual(displayed->getItemCount(), expected.size(), mOperation + ": displayed item count");
			for (size_t item = 0; item < expected.size(); ++item)
				checkEqual(
					displayed->getItemNameAt(item),
					expected[item],
					MyGUI::utility::toString(mOperation, ": displayed item ", item));
		}

		void testSorting()
		{
			RemoveAllItems();
			sortColumn(0, false);
			sortColumn(0, true);
			const MyGUI::UString names[] = {"beta", "alpha", "alpha", "gamma"};
			for (size_t item = 0; item < 4; ++item)
			{
				original_list->addItem(names[item], item);
				mirror_list.addItem(names[item], item);
				++count_items;
				for (size_t column = 1; column < count_columns; ++column)
				{
					const auto& name = names[(item + column) % 4];
					original_list->setSubItemNameAt(column, item, name);
					mirror_list.setSubItemNameAt(column, item, name);
					original_list->setSubItemDataAt(column, item, item + column * 10);
					mirror_list.setSubItemDataAt(column, item, item + column * 10);
				}
				if (item == 0)
				{
					sortColumn(0, false);
					sortColumn(0, true);
				}
			}
			original_list->setIndexSelected(1);
			for (size_t column = 0; column < count_columns; ++column)
			{
				sortColumn(column, false);
				sortColumn(column, true);
			}
			RemoveAllItems();
		}

		void SortItems()
		{
			if (count_columns == 0)
				return;
			const size_t column = mRandom() % count_columns;
			const bool descending = mRandom() % 2 != 0;
			sortColumn(column, descending);
		}

		void SortItems(size_t _count)
		{
			while (_count > 0)
			{
				SortItems();
				--_count;
			}
		}

		void ChangeItems()
		{
			mOperation = "ChangeItems";
			if (count_columns == 0)
				return;
			if (count_items == 0)
				return;

			size_t index = mRandom() % count_items;
			size_t column = mRandom() % count_columns;
			size_t item = mRandom();

			mirror_list.setSubItemNameAt(column, index, MyGUI::utility::toString(item));
			mirror_list.setSubItemDataAt(column, index, item);
			original_list->setSubItemNameAt(column, index, MyGUI::utility::toString(item));
			original_list->setSubItemDataAt(column, index, item);

			checkMultiList();
		}

		void ChangeItems(size_t _count)
		{
			while (_count > 0)
			{
				ChangeItems();
				--_count;
			}
		}

		void RemoveAllItems()
		{
			mOperation = "RemoveAllItems";
			if (count_columns == 0)
				return;

			mirror_list.removeAllItems();
			original_list->removeAllItems();

			count_items = 0;
			checkMultiList();
		}

		void randomStep()
		{
			if (count_items > 100)
			{
				RemoveAllItems();
			}

			size_t index = mRandom() % 7;
			size_t count = mRandom() % 3;

			if (index == 0)
				InsertItem(count);
			else if (index == 1)
				AddItem(count);
			else if (index == 2)
				RemoveItem(count);
			else if (index == 3)
				SwapItems(count);
			else if (index == 4)
				SortItems(count);
			else if (index == 5)
				ChangeItems(count * 5);
			else
				Begin(count);
		}
	};

}

#endif // UNITTEST_MULTILIST_H_
