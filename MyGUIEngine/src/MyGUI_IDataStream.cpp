/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_IDataStream.h"

namespace MyGUI
{

	namespace
	{

		template<typename Container>
		Container readAllData(IDataStream& _stream)
		{
			Container content;
			typename Container::value_type buffer[4096];
			while (size_t readSize = _stream.read(buffer, sizeof(buffer)))
				content.insert(content.end(), buffer, buffer + readSize);
			return content;
		}

	}

	std::vector<std::byte> IDataStream::readAll()
	{
		return readAllData<std::vector<std::byte>>(*this);
	}

	std::string IDataStream::readAllText()
	{
		return readAllData<std::string>(*this);
	}

} // namespace MyGUI
