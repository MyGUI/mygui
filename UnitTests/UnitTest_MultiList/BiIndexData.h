/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef BIINDEX_DATA_H_
#define BIINDEX_DATA_H_

#include "MyGUI_BiIndexBase.h"

namespace unittest
{

	class BiIndexData : public MyGUI::BiIndexBase
	{
	public:
		size_t getItemCount() const
		{
			return mItemsInfo.size();
		}

		void insertItemAt(size_t _index, std::string_view _name, size_t _data);

		void addItem(std::string_view _name, size_t _data)
		{
			insertItemAt(MyGUI::ITEM_NONE, _name, _data);
		}

		void removeItemAt(size_t _index);

		void removeAllItems();


		const std::string& getItemNameAt(size_t _index) const;

		void swapItemsAt(size_t _index1, size_t _index2);

		void swapSortItemsAt(size_t _index1, size_t _index2);

	private:
		using VectorString = std::vector<std::string>;

		// тестовые данные
		VectorString mItemsInfo;
	};

} // namespace unittest

#endif // BIINDEX_DATA_H_
