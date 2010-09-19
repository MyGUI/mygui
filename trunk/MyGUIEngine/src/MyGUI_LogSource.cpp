/*!
	@file
	@author		Albert Semenov
	@date		04/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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
