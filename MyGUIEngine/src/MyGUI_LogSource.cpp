/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LogSource.h"

namespace MyGUI
{

	void LogSource::setLogFilter(ILogFilter* _filter)
	{
		mFilter = _filter;
	}

	void LogSource::addLogListener(ILogListener* _lestener)
	{
		mListeners.push_back(_lestener);
	}

	void LogSource::open()
	{
		for (auto& listener : mListeners)
			listener->open();
	}

	void LogSource::close()
	{
		for (auto& listener : mListeners)
			listener->close();
	}

	void LogSource::flush()
	{
		for (auto& listener : mListeners)
			listener->flush();
	}

	void LogSource::log(
		std::string_view _section,
		LogLevel _level,
		const struct tm* _time,
		std::string_view _message,
		std::string_view _file,
		int _line)
	{
		if (mFilter != nullptr)
		{
			if (!mFilter->shouldLog(_section, _level, _time, _message, _file, _line))
				return;
		}

		for (auto& listener : mListeners)
			listener->log(_section, _level, _time, _message, _file, _line);
	}

} // namespace MyGUI
