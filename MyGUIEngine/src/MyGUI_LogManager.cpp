/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_LogManager.h"
#include <sstream>

namespace MyGUI
{

	std::string LogManager::General = "General";

	std::string LogManager::Critical = "Critical";
	std::string LogManager::Error = "Error";
	std::string LogManager::Warning = "Warning";
	std::string LogManager::Info = "Info";
	std::string LogManager::Debug = "Debug";

	std::string LogManager::separator = "  |  ";

	LogStreamEnd LogManager::endl;

	LogManager* LogManager::msInstance = 0;

	LogManager::LogManager()
	{
		msInstance = this;
	}

	LogManager::~LogManager()
	{
		for (MapLogStream::iterator iter=msInstance->mMapSectionFileName.begin(); iter!=msInstance->mMapSectionFileName.end(); ++iter) {
			delete iter->second;
		}
		msInstance->mMapSectionFileName.clear();
		msInstance = 0;
	}

	void LogManager::shutdown()
	{
		if (0 != msInstance) delete msInstance;
	}

	void LogManager::initialise()
	{
		if (0 == msInstance) new LogManager();
	}

	LogStream& LogManager::out(const std::string& _section, const std::string& _level)
	{
		lock();

		static LogStream empty;

		if (0 == msInstance) return empty;

		MapLogStream::iterator iter = msInstance->mMapSectionFileName.find(_section);
		if (iter == msInstance->mMapSectionFileName.end()) return empty;

		iter->second->start(_section, _level);
		release();

		return *(iter->second);
	}

	void LogManager::registerSection(const std::string& _section, const std::string& _file)
	{
		if (0 == msInstance) new LogManager();

		MapLogStream::iterator iter = msInstance->mMapSectionFileName.find(_section);
		if (iter != msInstance->mMapSectionFileName.end()) delete iter->second;

		msInstance->mMapSectionFileName[_section] = new LogStream(_file);
	}

	std::string LogManager::info(const char * _file /* = __FILE__*/, int _line /* = __LINE__*/)
	{
		std::string file="(_file)";
		file.reserve(200);
		/*size_t pos = file.find_last_of("\\/");
		if (pos != std::string::npos) file = file.substr(pos+1);*/

		//std::ostringstream stream;
        //stream << separator + _file + separator << _line;
		//file = file + separator;
		//file = separator + file;
		return file + file + file + file;
	}

} // namespace MyGUI
