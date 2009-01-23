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

	#define LOGGING(section, level, text) \
		MyGUI::LogManager::out(section, MyGUI::LogManager::level) \
		<< text \
		<< MyGUI::LogManager::info(__FILE__, __LINE__) \
		<< MyGUI::LogManager::end()

	class _MyGUIExport LogManager
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

	private:
		LogManager();
		~LogManager();

		inline static void lock()
		{
		}

		inline static void release()
		{
		}

	public:
		static const std::string General;
		static const std::string separator;

		static LogStreamEnd endl;
		static const std::string LevelsName[EndLogLevel];

	private:
		static LogManager * msInstance;
		MapLogStream mMapSectionFileName;

	};

} // namespace MyGUI

#endif // __MYGUI_LOG_MANAGER_H__
