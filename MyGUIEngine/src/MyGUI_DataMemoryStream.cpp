/*!
	@file
	@author		Albert Semenov
	@date		08/2012
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_DataMemoryStream.h"

namespace MyGUI
{

	DataMemoryStream::DataMemoryStream() :
		mData(nullptr),
		mSize(0),
		mStream(0)
	{
	}

	DataMemoryStream::DataMemoryStream(unsigned char* _data, size_t _size) :
		mData(_data),
		mSize(_size),
		mStream(nullptr)
	{
	}

	DataMemoryStream::~DataMemoryStream()
	{
		delete mStream;
	}

	size_t DataMemoryStream::size()
	{
		return mSize;
	}

	bool DataMemoryStream::eof()
	{
		if (mStream == nullptr)
			prepareStream();

		return mStream->eof();
	}

	void DataMemoryStream::readline(std::string& _source, Char _delim)
	{
		if (mStream == nullptr)
			prepareStream();

		std::getline(*mStream, _source, (char)_delim);
	}

	size_t DataMemoryStream::read(void* _buf, size_t _count)
	{
		if (mData == nullptr)
			return 0;

		size_t count = (std::min)(size(), _count);
		::memcpy(_buf, mData, count);
		return count;
	}

	void DataMemoryStream::prepareStream()
	{
		if (mData == nullptr)
			return;

		mStream = new std::stringstream((const char*)mData);
	}

} // namespace MyGUI
