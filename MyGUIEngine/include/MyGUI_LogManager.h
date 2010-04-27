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
//#include "MyGUI_Singleton.h"
#include "MyGUI_LogStream.h"
#include "MyGUI_LogSource.h"
#include "MyGUI_Diagnostic.h"
//#include <map>
#include <vector>

namespace MyGUI
{

	#define MYGUI_LOGGING(section, level, text) \
		MyGUI::LogManager::getInstance().log(section, MyGUI::LogLevel::level, MyGUI::LogStream() << text << MyGUI::LogStream::End(), __FILE__, __LINE__)

	class MYGUI_EXPORT LogManager// :
		//public Singleton<LogManager>
	{

	/*public:
		enum LogLevel
		{
			Info,
			Warning,
			Error,
			Critical,
			EndLogLevel
		};*/

	public:
		LogManager();
		~LogManager();

		//void initialise();
		//void shutdown();

		static LogManager& getInstance();
		static LogManager* getInstancePtr();

		// הכÿ סמגלוסעטלמסעט
		static void registerSection(const std::string& _section, const std::string& _fileName) { }
		static void unregisterSection(const std::string& _section) { }

		void flush();
		void log(const std::string& _section, LogLevel _level, const std::string& _message, const char* _file, int _line);

		void createDefaultListeners(const std::string& _logname);
		//static LogStream& out(const std::string& _section, LogLevel _level);
		//static const std::string& info(const char * _file /* = __FILE__*/, int _line /* = __LINE__*/);

		//static const LogStream::LogStreamEnd& end();

		// set logging enabled on std output device
		//static void setSTDOutputEnabled(bool _enable);
		//static bool getSTDOutputEnabled();

	//public:
		//static const std::string General;
		//static const std::string separator;

		//static LogStream::LogStreamEnd endl;
		//static const std::string LevelsName[EndLogLevel];
		void addLogSource(LogSource* _source);

	private:
		void close();

	private:
		static LogManager * msInstance;
		//typedef std::map<std::string, LogStream*>  MapLogStream;
		//MapLogStream mMapSectionFileName;
		//bool mSTDOut;

		typedef std::vector<LogSource*> VectorLogSource;
		VectorLogSource mSources;

		/*typedef std::map<std::string, ILogListener*> MapLogListener;
		MapLogListener mListeners;

		typedef std::map<std::string, ILogFilter*> MapLogFilter;
		MapLogFilter mFilter;*/
	};

} // namespace MyGUI

#endif // __MYGUI_LOG_MANAGER_H__
