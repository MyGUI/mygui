/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_DATA_STREAM_HOLDER_H_
#define MYGUI_DATA_STREAM_HOLDER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	/** Owns a stream released through the DataManager singleton. */
	class DataStreamHolder
	{
	public:
		/** Adopt a stream from the current DataManager. */
		DataStreamHolder(IDataStream* _data) noexcept :
			mData(_data)
		{
		}

		DataStreamHolder(const DataStreamHolder&) = delete;
		DataStreamHolder& operator=(const DataStreamHolder&) = delete;
		DataStreamHolder(DataStreamHolder&&) = delete;
		DataStreamHolder& operator=(DataStreamHolder&&) = delete;

		~DataStreamHolder()
		{
			DataManager::getInstance().freeData(mData);
		}

		IDataStream* getData() const
		{
			return mData;
		}

		IDataStream* operator->() const
		{
			return mData;
		}

		IDataStream& operator*() const
		{
			return *mData;
		}

		explicit operator bool() const
		{
			return mData != nullptr;
		}

	private:
		IDataStream* mData;
	};

} // namespace MyGUI

#endif // MYGUI_DATA_STREAM_HOLDER_H_
