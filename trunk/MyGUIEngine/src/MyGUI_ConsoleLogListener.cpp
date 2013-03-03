/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ConsoleLogListener.h"

namespace MyGUI
{

	ConsoleLogListener::ConsoleLogListener() :
		mEnabled(true)
	{
	}

	ConsoleLogListener::~ConsoleLogListener()
	{
	}

	void ConsoleLogListener::log(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line)
	{
		if (mEnabled)
			std::cout << _message << std::endl;
	}

	bool ConsoleLogListener::getEnabled() const
	{
		return mEnabled;
	}

	void ConsoleLogListener::setEnabled(bool _value)
	{
		mEnabled = _value;
	}

} // namespace MyGUI
