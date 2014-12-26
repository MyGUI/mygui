/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_BIINDEX_BASE_H_
#define MYGUI_BIINDEX_BASE_H_

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class MYGUI_EXPORT BiIndexBase
	{
	public:
		BiIndexBase();
		virtual ~BiIndexBase();

	protected:
		size_t getIndexCount() const;

		size_t insertItemAt(size_t _index);

		size_t removeItemAt(size_t _index);

		void removeAllItems();

		// на входе индексы пользователя, на выходе реальные индексы
		size_t convertToBack(size_t _index) const;

		// на входе индексы реальные, на выходе, то что видит пользователь
		size_t convertToFace(size_t _index) const;

		// меняет местами два индекса, индексы со стороны пользователя
		void swapItemsFaceAt(size_t _index1, size_t _index2);

		// меняет местами два индекса, индексы со сторонны данных
		void swapItemsBackAt(size_t _index1, size_t _index2);

#if MYGUI_DEBUG_MODE == 1
		void checkIndexes();
#endif

	private:
		typedef std::vector<size_t> VectorSizeT;

		// маппинг с индексов, которые видны наружу
		// на индексы которые реально используются данными
		VectorSizeT mIndexFace;

		// маппинг с индексов, которые используют данные
		// на индексы которые виндны наружу
		VectorSizeT mIndexBack;
	};

} // namespace MyGUI

#endif // MYGUI_BIINDEX_BASE_H_
