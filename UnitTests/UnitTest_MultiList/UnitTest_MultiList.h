/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __UNITTEST_MULTILIST_H__
#define __UNITTEST_MULTILIST_H__

#include "MyGUI.h"
#include "Mirror_MultiList.h"

namespace unittest
{
	class UnitTest_MultiList
	{
	private:
		MyGUI::MultiListPtr original_list;
		unittest::Mirror_MultiList * mirror_list;
		size_t count_items;
		size_t count_columns;

		void Assert(bool _expression) { if (!_expression) throw new std::exception(); }

	public:
		UnitTest_MultiList()
		{
			original_list = MyGUI::Gui::getInstance().createWidget<MyGUI::MultiList>("MultiList", MyGUI::IntCoord(300, 100, 400, 400), MyGUI::Align::Default, "Main");
			mirror_list = new unittest::Mirror_MultiList();
			count_items = 0;
			count_columns = 0;

			original_list->addColumn("1", 100);
			mirror_list->addColumn("1", 100);
			count_columns = 1;
		}

		~UnitTest_MultiList()
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(original_list);
			delete mirror_list;
		}

		void check()
		{
			Assert(count_items == original_list->getItemCount());
			Assert(count_columns == original_list->getColumnCount());

			Assert(original_list->getItemCount() == mirror_list->getItemCount());
			Assert(original_list->getColumnCount() == mirror_list->getColumnCount());

			for (size_t item=0; item<count_items; ++item) {
				for(size_t column=0; column<count_columns; ++column) {
					
					/*std::string name1 = original_list->getItemNameAt(item);
					std::string name2 = mirror_list->getItemNameAt(item);
					if (name1 != name2) {
						int test = 0;
					}*/
					Assert(original_list->getItemNameAt(item) == mirror_list->getItemNameAt(item));
					Assert(original_list->getSubItemNameAt(column, item) == mirror_list->getSubItemNameAt(column, item));

					Assert(((original_list->getItemDataAt<size_t>(item) == 0) && (mirror_list->getItemDataAt<size_t>(item) == 0))
						|| (*original_list->getItemDataAt<size_t>(item) == *mirror_list->getItemDataAt<size_t>(item)));
					Assert(original_list->getSubItemDataAt<size_t>(column, item) == 0 && mirror_list->getSubItemDataAt<size_t>(column, item) == 0
						|| *original_list->getSubItemDataAt<size_t>(column, item) == *mirror_list->getSubItemDataAt<size_t>(column, item));
				}
			}
		}

		void AddItem()
		{
			if (count_columns == 0) return;

			size_t item = (size_t)(Ogre::Math::UnitRandom() * (float)1000000);
			mirror_list->addItem(MyGUI::utility::toString(item), item);
			original_list->addItem(MyGUI::utility::toString(item), item);
			count_items ++;

			check();
		}

		void AddItem(size_t _count)
		{
			while (_count > 0) {
				AddItem();
				--_count;
			}
		}

		void InsertItem()
		{
			if (count_columns == 0) return;

			size_t index = count_items == 0 ? 0 : (size_t)(Ogre::Math::UnitRandom() * (float)1000000) % count_items;
			size_t item = (size_t)(Ogre::Math::UnitRandom() * (float)1000000);

			mirror_list->insertItemAt(index, MyGUI::utility::toString(item), item);
			original_list->insertItemAt(index, MyGUI::utility::toString(item), item);

			count_items ++;

			check();
		}

		void InsertItem(size_t _count)
		{
			while (_count > 0) {
				InsertItem();
				--_count;
			}
		}

		void RemoveItem()
		{
			if (count_columns == 0) return;
			if (count_items == 0) return;

			size_t index = count_items == 0 ? 0 : (size_t)(Ogre::Math::UnitRandom() * (float)1000000) % count_items;

			mirror_list->removeItemAt(index);
			original_list->removeItemAt(index);

			count_items --;

			check();
		}

		void RemoveItem(size_t _count)
		{
			while (_count > 0) {
				RemoveItem();
				--_count;
			}
		}

		void RemoveAllItems()
		{
			if (count_columns == 0) return;

			mirror_list->removeAllItems();
			original_list->removeAllItems();

			count_items = 0;
			check();
		}

		void nextFrame()
		{
			if (count_items > 300) {
				RemoveAllItems();
			}

			size_t index = (size_t)(Ogre::Math::UnitRandom() * (float)1000000) % 3;
			size_t count = (size_t)(Ogre::Math::UnitRandom() * (float)1000000) % 10;

			if (index == 0) InsertItem(count);
			else if (index == 1) AddItem(count);
			else if (index == 2) RemoveItem(count);
		}

	};

}

#endif // __UNITTEST_MULTILIST_H__
