/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef __UNITTEST_MULTILIST_H__
#define __UNITTEST_MULTILIST_H__

#include "MyGUI.h"
#include "Mirror_MultiList.h"
#include "BiIndexData.h"

namespace unittest
{
	class UnitTest_MultiList
	{
	private:
		MyGUI::MultiListBox* original_list;
		//unittest::BiIndexData * original_list;
		unittest::Mirror_MultiList* mirror_list;
		size_t count_items;
		size_t count_columns;

		void Assert(bool _expression)
		{
			if (!_expression)
			{
				throw new std::exception();
			}
		}

	public:
		UnitTest_MultiList()
		{
			original_list = MyGUI::Gui::getInstance().createWidget<MyGUI::MultiListBox>("MultiListBox", MyGUI::IntCoord(300, 100, 400, 400), MyGUI::Align::Default, "Main");
			//original_list = new unittest::BiIndexData();
			mirror_list = new unittest::Mirror_MultiList();
			count_items = 0;
			count_columns = 0;

			original_list->addColumn("1");
			original_list->setColumnResizingPolicyAt(0, MyGUI::ResizingPolicy::Fill);
			mirror_list->addColumn("1");
			count_columns = 1;

			original_list->addColumn("2");
			original_list->setColumnResizingPolicyAt(1, MyGUI::ResizingPolicy::Fill);
			mirror_list->addColumn("2");
			count_columns = 2;

			original_list->addColumn("3");
			original_list->setColumnResizingPolicyAt(2, MyGUI::ResizingPolicy::Fill);
			mirror_list->addColumn("3");
			count_columns = 3;

			//start();
		}

		~UnitTest_MultiList()
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(original_list);
			//delete original_list;
			//mirror_list->removeAllColumns();
			delete mirror_list;
		}

		void checkMultiList()
		{
			Assert(count_items == original_list->getItemCount());
			Assert(count_columns == original_list->getColumnCount());

			Assert(original_list->getItemCount() == mirror_list->getItemCount());
			Assert(original_list->getColumnCount() == mirror_list->getColumnCount());

			for (size_t item = 0; item < count_items; ++item)
			{
				for (size_t column = 0; column < count_columns; ++column)
				{

					//Assert(MyGUI::UString(original_list->getItemNameAt(item)) == mirror_list->getItemNameAt(item));
					//Assert(original_list->getItemNameAt(item) == MyGUI::utility::toString(item));

					//Assert(MyGUI::UString(original_list->getItemNameAt(item)) == mirror_list->getItemNameAt(item));
					Assert(original_list->getItemNameAt(item) == mirror_list->getItemNameAt(item));

					Assert(original_list->getSubItemNameAt(column, item) == mirror_list->getSubItemNameAt(column, item));

					Assert(((original_list->getItemDataAt<size_t>(item, false) == 0) && (mirror_list->getItemDataAt<size_t>(item, false) == 0))
						|| (*original_list->getItemDataAt<size_t>(item, false) == *mirror_list->getItemDataAt<size_t>(item, false)));

					Assert((original_list->getSubItemDataAt<size_t>(column, item, false) == 0 && mirror_list->getSubItemDataAt<size_t>(column, item, false) == 0)
						|| *original_list->getSubItemDataAt<size_t>(column, item, false) == *mirror_list->getSubItemDataAt<size_t>(column, item, false));
				}
			}
		}

		void Begin()
		{
			if (count_columns == 0) return;
			size_t count = original_list->getItemCount();
			if (count == 0) return;

			size_t index = (size_t)rand() % count;
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
			if (count_columns == 0) return;

			size_t item = (size_t)rand();
			mirror_list->addItem(MyGUI::utility::toString(item), item);
			original_list->addItem(MyGUI::utility::toString(item), item);
			count_items ++;

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
			if (count_columns == 0) return;

			size_t index = count_items == 0 ? 0 : ((size_t)rand() % count_items);
			size_t item = (size_t)rand();

			mirror_list->insertItemAt(index, MyGUI::utility::toString(item), item);
			original_list->insertItemAt(index, MyGUI::utility::toString(item), item);

			count_items ++;

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
			if (count_columns == 0) return;
			if (count_items == 0) return;

			size_t index = count_items == 0 ? 0 : ((size_t)rand() % count_items);

			mirror_list->removeItemAt(index);
			original_list->removeItemAt(index);

			count_items --;

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
			if (count_columns == 0) return;
			if (count_items == 0) return;

			size_t index1 = count_items == 0 ? 0 : ((size_t)rand() % count_items);
			size_t index2 = count_items == 0 ? 0 : ((size_t)rand() % count_items);

			mirror_list->swapItemsAt(index1, index2);
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

		void SortItems()
		{
			if (count_columns == 0) return;
			if (count_items == 0) return;

			//size_t index1 = count_items == 0 ? 0 : ((size_t)rand() % count_items);
			//size_t index2 = count_items == 0 ? 0 : ((size_t)rand() % count_items);

			//mirror_list->swapItemsAt(index1, index2);
			//original_list->swapSortItemsAt(index1, index2);

			checkMultiList();
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
			if (count_columns == 0) return;
			if (count_items == 0) return;

			size_t index = (size_t)rand() % count_items;
			size_t column = (size_t)rand() % count_columns;
			size_t item = (size_t)rand();

			mirror_list->setSubItemNameAt(column, index, MyGUI::utility::toString(item));
			mirror_list->setSubItemDataAt(column, index, item);
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
			if (count_columns == 0) return;

			mirror_list->removeAllItems();
			original_list->removeAllItems();

			count_items = 0;
			checkMultiList();
		}

		/*void start()
		{
			count_items = original_list->generate();
			for (size_t pos=0; pos<count_items; ++pos) {
				mirror_list->addItem(MyGUI::utility::toString(pos));
			}
		}*/

		void nextFrame()
		{
			/*static bool in = false;
			if (in) return;
			in = true;

			original_list->insertItemAt(0, "1");
			original_list->insertItemAt(0, "2");
			original_list->insertItemAt(2, "3");
			original_list->removeItemAt(0);
			original_list->removeItemAt(0);
			original_list->removeItemAt(0);
			original_list->insertItemAt(0, "4");
			original_list->removeItemAt(0);
			original_list->insertItemAt(0, "5");
			original_list->insertItemAt(0, "6");
			original_list->insertItemAt(2, "7");
			original_list->insertItemAt(3, "8");
			original_list->insertItemAt(4, "9");
			original_list->removeItemAt(1);
			original_list->removeItemAt(0);*/

			if (count_items > 100)
			{
				RemoveAllItems();
			}

			size_t index = (size_t)rand() % 7;
			size_t count = (size_t)rand() % 3;

			if (index == 0) InsertItem(count);
			else if (index == 1) AddItem(count);
			else if (index == 2) RemoveItem(count);
			else if (index == 3) SwapItems(count);
			else if (index == 4) SortItems(count);
			else if (index == 5) ChangeItems(count * 5);
			else if (index == 6) Begin(count);

			//base::BaseManager::getInstance().getStatisticInfo()->change("Count", count_items);
		}

	};

}

#endif // __UNITTEST_MULTILIST_H__
