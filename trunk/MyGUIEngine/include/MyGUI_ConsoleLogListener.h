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
#ifndef __MYGUI_CONSOLE_LOG_LISTENER_H__
#define __MYGUI_CONSOLE_LOG_LISTENER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ILogListener.h"

namespace MyGUI
{
	class MYGUI_EXPORT ConsoleLogListener :
		public ILogListener
	{
	public:
		ConsoleLogListener();
		virtual ~ConsoleLogListener() { }

		//! @copydoc ILogListener::log(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line)
		virtual void log(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line);

		// DESCRIBEME
		// устанавливает доступность подписчика
		bool getEnable() { return mEnable; }
		// DESCRIBEME
		// возвращает доступность подписчика
		void setEnable(bool _value) { mEnable = _value; }

	private:
		bool mEnable;
	};
}

#endif // __MYGUI_CONSOLE_LOG_LISTENER_H__
