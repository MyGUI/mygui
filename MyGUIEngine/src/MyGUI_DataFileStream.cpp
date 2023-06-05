/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_DataFileStream.h"

namespace MyGUI
{

	DataFileStream::DataFileStream(std::unique_ptr<std::ifstream>&& _stream) :
		DataStream(_stream.get()),
		mFileStream(std::move(_stream))
	{
	}

	DataFileStream::~DataFileStream()
	{
		if (mFileStream)
			mFileStream->close();
	}

} // namespace MyGUI
