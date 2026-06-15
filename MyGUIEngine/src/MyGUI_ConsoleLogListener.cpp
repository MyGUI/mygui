/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ConsoleLogListener.h"

namespace MyGUI
{

	void ConsoleLogListener::log(
		std::string_view /*_section*/,
		LogLevel /*unused*/,
		const struct tm* /*unused*/,
		std::string_view _message,
		std::string_view /*unused*/,
		int /*unused*/)
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
