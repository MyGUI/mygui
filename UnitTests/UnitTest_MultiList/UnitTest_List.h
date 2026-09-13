/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef UNITTEST_LIST_H_
#define UNITTEST_LIST_H_

#include "MyGUI.h"
#include "ListTestChecks.h"
#include <random>
#include "Mirror_List.h"

namespace unittest
{

	class UnitTest_List
	{
	private:
		MyGUI::ListBox* original_list;
		Mirror_List mirror_list;
		std::mt19937& mRandom;
		std::string mOperation{"initial state"};
		size_t count_items;

	public:
		explicit UnitTest_List(std::mt19937& _random) :
			mRandom(_random)
		{
			original_list = MyGUI::Gui::getInstance().createWidget<MyGUI::ListBox>(
				"Default",
				MyGUI::IntCoord(100, 100, 100, 100),
				MyGUI::Align::Default,
				"Main");
			count_items = 0;
		}

		~UnitTest_List()
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(original_list);
		}

		void checkList()
		{
			checkEqual(original_list->getItemCount(), count_items, mOperation + ": ListBox item count");
			checkEqual(mirror_list.getItemCount(), count_items, mOperation + ": reference item count");
			for (size_t item = 0; item < count_items; ++item)
			{
				const auto where = MyGUI::utility::toString(mOperation, ": ListBox item ", item);
				checkEqual(original_list->getItemNameAt(item), mirror_list.getItemNameAt(item), where + " name");
				checkData(
					original_list->getItemDataAt<size_t>(item, false),
					mirror_list.getItemDataAt<size_t>(item, false),
					where + " data");
			}
			original_list->_checkAlign();
		}

		void Begin()
		{
			mOperation = "Begin";
			size_t count = original_list->getItemCount();
			if (count == 0)
				return;
			size_t index = (mRandom() % count);
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
			mOperation = "AddItem";
			size_t item = mRandom();
			mirror_list.addItem(MyGUI::utility::toString(item), item);
			original_list->addItem(MyGUI::utility::toString(item), item);
			count_items++;

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
			mOperation = "InsertItem";
			size_t index = count_items == 0 ? 0 : (mRandom() % count_items);
			size_t item = mRandom();

			mirror_list.insertItemAt(index, MyGUI::utility::toString(item), item);
			original_list->insertItemAt(index, MyGUI::utility::toString(item), item);

			count_items++;

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
			mOperation = "RemoveItem";
			if (count_items == 0)
				return;

			size_t index = mRandom() % count_items;

			mirror_list.removeItemAt(index);
			original_list->removeItemAt(index);

			count_items--;

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
			mOperation = "RemoveAllItems";
			mirror_list.removeAllItems();
			original_list->removeAllItems();

			count_items = 0;
			checkList();
		}

		void randomStep()
		{
			if (count_items > 100)
				RemoveAllItems();

			size_t index = mRandom() % 4;
			size_t count = mRandom() % 3;

			if (index == 0)
				InsertItem(count);
			else if (index == 1)
				AddItem(count);
			else if (index == 2)
				RemoveItem(count);
			else
				Begin(count);
		}
	};

}

#endif // UNITTEST_LIST_H_
