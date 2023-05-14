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

	bool LevelLogFilter::shouldLog(std::string_view, LogLevel _level, const struct tm*, std::string_view, std::string_view, int)
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
