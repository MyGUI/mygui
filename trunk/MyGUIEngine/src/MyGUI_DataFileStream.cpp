/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_DataFileStream.h"

namespace MyGUI
{

	DataFileStream::DataFileStream() :
		DataStream(),
		mFileStream(nullptr)
	{
	}

	DataFileStream::DataFileStream(std::ifstream* _stream) :
		DataStream(_stream),
		mFileStream(_stream)
	{
	}

	DataFileStream::~DataFileStream()
	{
		if (mFileStream != nullptr)
		{
			mFileStream->close();
			delete mFileStream;
			mFileStream = nullptr;
		}
	}

} // namespace MyGUI
