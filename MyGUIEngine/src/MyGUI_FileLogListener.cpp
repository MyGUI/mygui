/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_FileLogListener.h"
#include "MyGUI_FileSystemUtility.h"
#include <iomanip>
#include <ctime>

namespace MyGUI
{

	void FileLogListener::open()
	{
		mStream.open(utility::pathFromUTF8(mFileName), std::ios_base::out);
	}

	void FileLogListener::close()
	{
		if (mStream.is_open())
			mStream.close();
	}

	void FileLogListener::flush()
	{
		if (mStream.is_open())
			mStream.flush();
	}

	void FileLogListener::log(
		std::string_view _section,
		LogLevel _level,
		const struct tm* _time,
		std::string_view _message,
		std::string_view _file,
		int _line)
	{
		if (mStream.is_open())
		{
			std::string_view separator = "  |  ";
			mStream << std::setw(2) << std::setfill('0') << _time->tm_hour << ":" << std::setw(2) << std::setfill('0')
					<< _time->tm_min << ":" << std::setw(2) << std::setfill('0') << _time->tm_sec << separator
					<< _section << separator << _level.print() << separator << _message << separator << _file
					<< separator << _line << std::endl;
		}
	}

	void FileLogListener::setFileName(std::string_view _value)
	{
		mFileName = _value;
	}

	const std::string& FileLogListener::getFileName() const
	{
		return mFileName;
	}

} // namespace MyGUI
