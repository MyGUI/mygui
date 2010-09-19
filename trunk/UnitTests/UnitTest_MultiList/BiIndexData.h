/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef __BIINDEX_DATA_H__
#define __BIINDEX_DATA_H__

#include "MyGUI_BiIndexBase.h"

namespace unittest
{

	class BiIndexData :
		public MyGUI::BiIndexBase
	{
	public:
		size_t getItemCount() const
		{
			return mItemsInfo.size();
		}

		void insertItemAt(size_t _index, const std::string& _name, size_t _data);

		void addItem(const std::string& _name, size_t _data)
		{
			insertItemAt(MyGUI::ITEM_NONE, _name, _data);
		}

		void removeItemAt(size_t _index);

		void removeAllItems();


		const std::string& getItemNameAt(size_t _index);

		void swapItemsAt(size_t _index1, size_t _index2);

		void swapSortItemsAt(size_t _index1, size_t _index2);

	private:
		typedef std::vector<std::string> VectorString;

		// тестовые данные
		VectorString mItemsInfo;

	};

} // namespace unittest

#endif // __BIINDEX_DATA_H__
