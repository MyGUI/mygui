/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
