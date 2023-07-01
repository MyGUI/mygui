/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_CONSOLE_LOG_LISTENER_H_
#define MYGUI_CONSOLE_LOG_LISTENER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ILogListener.h"

namespace MyGUI
{

	class MYGUI_EXPORT ConsoleLogListener : public ILogListener
	{
	public:
		//! @copydoc ILogListener::log(std::string_view _section, LogLevel _level, const struct tm* _time, std::string_view _message, std::string_view _file, int _line)
		void log(
			std::string_view _section,
			LogLevel _level,
			const struct tm* _time,
			std::string_view _message,
			std::string_view _file,
			int _line) override;

		/** Is log listener enabled. */
		bool getEnabled() const;
		/** Enable or disable log listener. */
		void setEnabled(bool _value);

	private:
		bool mEnabled{true};
	};

} // namespace MyGUI

#endif // MYGUI_CONSOLE_LOG_LISTENER_H_
