/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LevelLogFilter.h"

namespace MyGUI
{

	LevelLogFilter::LevelLogFilter() :
		mLevel(LogLevel::Info)
	{
	}

	LevelLogFilter::~LevelLogFilter()
	{
	}

	bool LevelLogFilter::shouldLog(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line)
	{
		return mLevel <= _level;
	}

	void LevelLogFilter::setLoggingLevel(LogLevel _value)
	{
		mLevel = _value;
	}

	LogLevel LevelLogFilter::getLoggingLevel() const
	{
		return mLevel;
	}

} // namespace MyGUI
