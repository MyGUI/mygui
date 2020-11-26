/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_DATA_MEMORY_STREAM_H_
#define MYGUI_DATA_MEMORY_STREAM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IDataStream.h"

namespace MyGUI
{

	class MYGUI_EXPORT DataMemoryStream :
		public IDataStream
	{
	public:
		DataMemoryStream();
		DataMemoryStream(unsigned char* _data, size_t _size);
		~DataMemoryStream() override;

		bool eof() override;
		size_t size() override;
		void readline(std::string& _source, Char _delim) override;
		size_t read(void* _buf, size_t _count) override;

	private:
		void prepareStream();

	protected:
		unsigned char* mData;
		size_t mSize;
		std::stringstream* mStream;
	};

} // namespace MyGUI

#endif // MYGUI_DATA_MEMORY_STREAM_H_
