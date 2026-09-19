/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_DataStream.h"

#include <limits>

namespace MyGUI
{

	DataStream::DataStream() :
		mStream(nullptr),
		mSize((size_t)-1)
	{
	}

	DataStream::DataStream(std::istream* _stream) :
		mStream(_stream),
		mSize((size_t)-1)
	{
	}

	size_t DataStream::size()
	{
		if (mStream == nullptr || mStream->rdbuf() == nullptr)
			return 0;
		if (mSize == (size_t)-1)
		{
			// Query the buffer directly to preserve stream flags, including EOF and errors.
			auto* buffer = mStream->rdbuf();
			const auto position = buffer->pubseekoff(0, std::ios::cur, std::ios::in);
			if (position == std::streampos(-1))
				return 0;
			const auto end = buffer->pubseekoff(0, std::ios::end, std::ios::in);
			const auto restored = buffer->pubseekpos(position, std::ios::in);
			if (end == std::streampos(-1) || restored == std::streampos(-1))
				return 0;
			mSize = static_cast<size_t>(end);
		}
		return mSize;
	}

	bool DataStream::eof()
	{
		return mStream == nullptr ? true : mStream->eof();
	}

	void DataStream::readline(std::string& _source, Char _delim)
	{
		if (mStream == nullptr)
			return;
		std::getline(*mStream, _source, (char)_delim);
	}

	size_t DataStream::read(void* _buf, size_t _count)
	{
		if (mStream == nullptr || _count == 0)
			return 0;
		const auto count = std::min(_count, static_cast<size_t>(std::numeric_limits<std::streamsize>::max()));
		mStream->read(static_cast<char*>(_buf), static_cast<std::streamsize>(count));
		return static_cast<size_t>(mStream->gcount());
	}

} // namespace MyGUI
