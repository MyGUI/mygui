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

	class DataStreamHolder
	{
	public:
		DataStreamHolder(IDataStream* _data) :
			mData(_data)
		{
		}

		~DataStreamHolder()
		{
			DataManager::getInstance().freeData(mData);
		}

		IDataStream* getData()
		{
			return mData;
		}

	private:
		IDataStream* mData;
	};

} // namespace MyGUI

#endif // MYGUI_DATA_STREAM_HOLDER_H_
