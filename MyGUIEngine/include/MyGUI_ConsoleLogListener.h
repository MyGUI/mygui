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

	class MYGUI_EXPORT ConsoleLogListener :
		public ILogListener
	{
	public:
		ConsoleLogListener();
		virtual ~ConsoleLogListener();

		//! @copydoc ILogListener::log(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line)
		virtual void log(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line);

		/** Is log listener enabled. */
		bool getEnabled() const;
		/** Enable or disable log listener. */
		void setEnabled(bool _value);

	private:
		bool mEnabled;
	};

} // namespace MyGUI

#endif // MYGUI_CONSOLE_LOG_LISTENER_H_
