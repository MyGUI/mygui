/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_DataStream.h"

namespace MyGUI
{

	DataStream::DataStream() :
		mStream(nullptr),
		mSize((size_t) - 1)
	{
	}

	DataStream::DataStream(std::istream* _stream) :
		mStream(_stream),
		mSize((size_t) - 1)
	{
	}

	DataStream::~DataStream()
	{
	}

	size_t DataStream::size()
	{
		if (mStream == nullptr) return 0;
		if (mSize == (size_t) - 1)
		{
			mStream->seekg (0, std::ios::end);
			mSize = (size_t)mStream->tellg();
			mStream->seekg (0, std::ios::beg);
		}
		return mSize;
	}

	bool DataStream::eof()
	{
		return mStream == nullptr ? true : mStream->eof();
	}

	void DataStream::readline(std::string& _source, Char _delim)
	{
		if (mStream == nullptr) return;
		std::getline(*mStream, _source, (char)_delim);
	}

	size_t DataStream::read(void* _buf, size_t _count)
	{
		if (mStream == nullptr) return 0;
		size_t count = std::min(size(), _count);
		mStream->read((char*)_buf, count);
		return count;
	}

} // namespace MyGUI
