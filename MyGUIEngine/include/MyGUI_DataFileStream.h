/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_DATA_FILE_STREAM_H_
#define MYGUI_DATA_FILE_STREAM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataStream.h"
#include <memory>

namespace MyGUI
{

	class MYGUI_EXPORT DataFileStream : public DataStream
	{
	public:
		DataFileStream() = default;
		DataFileStream(std::unique_ptr<std::ifstream>&& _stream);
		~DataFileStream() override;

	private:
		std::unique_ptr<std::ifstream> mFileStream;
	};

} // namespace MyGUI

#endif // MYGUI_DATA_STREAM_H_
