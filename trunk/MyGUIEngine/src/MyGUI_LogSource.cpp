/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LogSource.h"

namespace MyGUI
{

	LogSource::LogSource() :
		mFilter(nullptr)
	{
	}

	LogSource::~LogSource()
	{
	}

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
		for (VectorLogListeners::iterator listener = mListeners.begin(); listener != mListeners.end(); ++listener)
			(*listener)->open();
	}

	void LogSource::close()
	{
		for (VectorLogListeners::iterator listener = mListeners.begin(); listener != mListeners.end(); ++listener)
			(*listener)->close();
	}

	void LogSource::flush()
	{
		for (VectorLogListeners::iterator listener = mListeners.begin(); listener != mListeners.end(); ++listener)
			(*listener)->flush();
	}

	void LogSource::log(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line)
	{
		if (mFilter != nullptr)
		{
			if (!mFilter->shouldLog(_section, _level, _time, _message, _file, _line))
				return;
		}

		for (VectorLogListeners::iterator listener = mListeners.begin(); listener != mListeners.end(); ++listener)
			(*listener)->log(_section, _level, _time, _message, _file, _line);
	}

} // namespace MyGUI
