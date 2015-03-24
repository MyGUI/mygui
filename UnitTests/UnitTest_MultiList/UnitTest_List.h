/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef UNITTEST_LIST_H_
#define UNITTEST_LIST_H_

#include "MyGUI.h"
#include "Mirror_List.h"

namespace unittest
{
	class UnitTest_List
	{
	private:
		MyGUI::ListBox* original_list;
		unittest::Mirror_List* mirror_list;
		size_t count_items;

	public:
		UnitTest_List()
		{
			original_list = MyGUI::Gui::getInstance().createWidget<MyGUI::ListBox>("ListBox", MyGUI::IntCoord(100, 100, 100, 100), MyGUI::Align::Default, "Main");
			mirror_list = new unittest::Mirror_List();
			count_items = 0;
		}

		~UnitTest_List()
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(original_list);
			delete mirror_list;
		}

		void checkList()
		{
			assert(count_items == original_list->getItemCount());
			assert(original_list->getItemCount() == mirror_list->getItemCount());

			for (size_t pos = 0; pos < count_items; ++pos)
			{
				assert(original_list->getItemNameAt(pos) == mirror_list->getItemNameAt(pos));
				assert(*original_list->getItemDataAt<size_t>(pos) == *mirror_list->getItemDataAt<size_t>(pos));
			}

			original_list->_checkAlign();
		}

		void Begin()
		{
			size_t count = original_list->getItemCount();
			if (count == 0) return;
			size_t index = ((size_t)rand() % count);
			original_list->beginToItemAt(index);

			checkList();
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
			size_t item = (size_t)rand();
			mirror_list->addItem(MyGUI::utility::toString(item), item);
			original_list->addItem(MyGUI::utility::toString(item), item);
			count_items ++;

			checkList();
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
			size_t index = count_items == 0 ? 0 : ((size_t)rand() % count_items);
			size_t item = (size_t)rand();

			mirror_list->insertItemAt(index, MyGUI::utility::toString(item), item);
			original_list->insertItemAt(index, MyGUI::utility::toString(item), item);

			count_items ++;

			checkList();
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
			if (count_items == 0) return;

			size_t index = count_items == 0 ? 0 : ((size_t)rand() % count_items);

			mirror_list->removeItemAt(index);
			original_list->removeItemAt(index);

			count_items --;

			checkList();
		}

		void RemoveItem(size_t _count)
		{
			while (_count > 0)
			{
				RemoveItem();
				--_count;
			}
		}

		void RemoveAllItems()
		{
			mirror_list->removeAllItems();
			original_list->removeAllItems();

			count_items = 0;
			checkList();
		}

		void nextFrame()
		{
			if (count_items > 100) RemoveAllItems();

			size_t index = (size_t)rand() % 4;
			size_t count = (size_t)rand() % 3;

			if (index == 0) InsertItem(count);
			else if (index == 1) AddItem(count);
			else if (index == 2) RemoveItem(count);
			else if (index == 3) Begin(count);
		}

	};

}

#endif // UNITTEST_LIST_H_
