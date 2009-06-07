/*!
	@file
	@author		Albert Semenov
	@date		05/2009
	@module
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
#ifndef __MYGUI_DATA_H__
#define __MYGUI_DATA_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class MYGUI_EXPORT Data
	{
	public:
		Data() :
			mData(nullptr),
			mSize(0)
		{
		}

		~Data()
		{
			release();
		}

		void setData(uint8* _data, size_t _size)
		{
			release();
			mData = _data;
			mSize = _size;
		}

		void setSize(size_t _size)
		{
			release();
			if (_size)
			{
				mSize = _size;
				mData = new uint8[mSize];
			}
		}

		void release()
		{
			if (mData != nullptr)
			{
				delete[] mData;
				mData = nullptr;
				mSize = 0;
			}
		}

		uint8* getData() { return mData; }
		size_t getSize() { return mSize; }

	private:
		uint8* mData;
		size_t mSize;
	};

} // namespace MyGUI

#endif // __MYGUI_DATA_H__
