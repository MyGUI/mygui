/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_BIINDEX_BASE_H_
#define MYGUI_BIINDEX_BASE_H_

#include "MyGUI_Prerequest.h"
#include <vector>
#include <stddef.h>

namespace MyGUI
{

	class MYGUI_EXPORT BiIndexBase
	{
	public:
		virtual ~BiIndexBase() = default;

	protected:
		size_t getIndexCount() const;

		size_t insertItemAt(size_t _index);

		size_t removeItemAt(size_t _index);

		void removeAllItems();

		// Convert from user-facing index to actual data index
		size_t convertToBack(size_t _index) const;

		// Convert from actual data index to user-facing index
		size_t convertToFace(size_t _index) const;

		// Swap two items by user-facing indices
		void swapItemsFaceAt(size_t _index1, size_t _index2);

		// Swap two items by actual data indices
		void swapItemsBackAt(size_t _index1, size_t _index2);

		void checkIndexes();

	private:
		using VectorSizeT = std::vector<size_t>;

		// Mapping from user-facing indices to actual data indices
		VectorSizeT mIndexFace;

		// Mapping from actual data indices to user-facing indices
		// to user-visible indices
		VectorSizeT mIndexBack;
	};

} // namespace MyGUI

#endif // MYGUI_BIINDEX_BASE_H_
