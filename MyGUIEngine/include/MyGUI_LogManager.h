/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_LOG_MANAGER_H__
#define __MYGUI_LOG_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_LogStream.h"

namespace MyGUI
{

	#define MYGUI_LOGGING(section, level, text) \
		MyGUI::LogManager::out(section, MyGUI::LogManager::level) \
		<< text \
		<< MyGUI::LogManager::info(__FILE__, __LINE__) \
		<< MyGUI::LogManager::end()

	class LogStream;
	struct LogStreamEnd;
	typedef std::map<std::string, LogStream*> MapLogStream;

	class MYGUI_EXPORT LogManager
	{

	public:
		enum LogLevel
		{
			Info,
			Warning,
			Error,
			Critical,
			EndLogLevel
		};

	public:
		static void shutdown();
		static void initialise();

		static void registerSection(const std::string& _section, const std::string& _file);
		static void unregisterSection(const std::string& _section);

		static LogStream& out(const std::string& _section, LogLevel _level);
		static const std::string& info(const char * _file /* = __FILE__*/, int _line /* = __LINE__*/);

		static const LogStreamEnd& end();

		// set logging enabled on std output device
		static void setSTDOutputEnabled(bool _enable);
		static bool getSTDOutputEnabled();

	private:
		LogManager();
		~LogManager();

	public:
		static const std::string General;
		static const std::string separator;

		static LogStreamEnd endl;
		static const std::string LevelsName[EndLogLevel];

	private:
		static LogManager * msInstance;
		MapLogStream mMapSectionFileName;
		bool mSTDOut;
	};

} // namespace MyGUI

#endif // __MYGUI_LOG_MANAGER_H__
