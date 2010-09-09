/*!
	@file
	@author		Albert Semenov
	@date		01/2008
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
#ifndef __MYGUI_LOG_MANAGER_H__
#define __MYGUI_LOG_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_LogStream.h"
#include "MyGUI_LogSource.h"
#include "MyGUI_Diagnostic.h"
#include <vector>

namespace MyGUI
{

	#define MYGUI_LOGGING(section, level, text) \
		MyGUI::LogManager::getInstance().log(section, MyGUI::LogLevel::level, MyGUI::LogStream() << text << MyGUI::LogStream::End(), __FILE__, __LINE__)

	class ConsoleLogListener;
	class FileLogListener;
	class LevelLogFilter;

	class MYGUI_EXPORT LogManager
	{
	public:
		LogManager();
		~LogManager();

		static LogManager& getInstance();
		static LogManager* getInstancePtr();

		// DESCRIBEME
		// рассылает всем источникам flush
		void flush();
		// DESCRIBEME
		// рассылает всем источникам лог евент
		void log(const std::string& _section, LogLevel _level, const std::string& _message, const char* _file, int _line);

		// DESCRIBEME
		// создает дефолтный источник в который входит :
		// 1. LevelLogFilter - фильтр по уровню события лога
		// 2. FileLogListener - подписчик для записи в файл
		// 3. ConsoleLogListener - подписчик для записи в std::cout
		void createDefaultSource(const std::string& _logname);

		// DESCRIBEME
		// устанавливает доступность дефолтного подписчика в std::cout
		void setSTDOutputEnabled(bool _value);
		// DESCRIBEME
		// возвращает доступность дефолтного подписчика в std::cout
		bool getSTDOutputEnabled() const;

		// DESCRIBEME
		// устанавливает уровень для дефолтного фильтра
		void setLoggingLevel(LogLevel _value);
		// DESCRIBEME
		// возвращает уровень для дефолтного фильтра
		LogLevel getLoggingLevel() const;

		// DESCRIBEME
		// добавляет источник в список источников
		void addLogSource(LogSource* _source);

	private:
		void close();

	private:
		static LogManager* msInstance;

		typedef std::vector<LogSource*> VectorLogSource;
		VectorLogSource mSources;

		ConsoleLogListener* mConsole;
		FileLogListener* mFile;
		LevelLogFilter* mFilter;
		LogSource* mDefaultSource;

		LogLevel mLevel;
		bool mConsoleEnable;
	};

} // namespace MyGUI

#endif // __MYGUI_LOG_MANAGER_H__
