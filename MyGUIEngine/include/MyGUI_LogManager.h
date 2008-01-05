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

	//const LogStreamEnd endl;

	class _MyGUIExport LogManager
	{
	public:
		static void shutdown();
		static void initialise();

		static LogStream& out(const std::string& _section, const std::string& _level);
		static void registerSection(const std::string& _section, const std::string& _file);

		static const std::string& info(const char * _file /* = __FILE__*/, int _line /* = __LINE__*/);

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
		//Sections
		static const std::string General;

		// Levels
		static const std::string Critical;
		static const std::string Error;
		static const std::string Warning;
		static const std::string Info;
		static const std::string Debug;

		static const std::string separator;

		static int Test;

	private:
		static LogManager * msInstance;
		MapLogStream mMapSectionFileName;

	};

} // namespace MyGUI

#endif // __MYGUI_LOG_MANAGER_H__
