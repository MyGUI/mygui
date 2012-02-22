/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_BIINDEX_BASE_H__
#define __MYGUI_BIINDEX_BASE_H__

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

#endif // __MYGUI_BIINDEX_BASE_H__
