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
#include "MyGUI_Precompiled.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_FileLogListener.h"
#include "MyGUI_ConsoleLogListener.h"
#include "MyGUI_LogSource.h"
//#include <sstream>
#include <time.h>

namespace MyGUI
{

	//template <> const char* Singleton<LogManager>::INSTANCE_TYPE_NAME("LogManager");
	/*const std::string LogManager::LevelsName[EndLogLevel] =
	{
		"Info",
		"Warning",
		"Error",
		"Critical"
	};*/

	//const std::string LogManager::General = "General";
	//const std::string LogManager::separator = "  |  ";

	//LogStream::LogStreamEnd LogManager::endl;
	LogManager* LogManager::msInstance = nullptr;

	LogManager::LogManager()
	{
		msInstance = this;
		/*mSTDOut = true;*/
	}

	LogManager::~LogManager()
	{
		flush();
		close();

		msInstance = nullptr;
		/*MapLogStream& mapStream = msInstance->mMapSectionFileName;
		for (MapLogStream::iterator iter=mapStream.begin(); iter!=mapStream.end(); ++iter)
		{
			LogStream * stream = iter->second;
			if (stream == 0) continue;

			// ищем все такие потоки и обнуляем
			for (MapLogStream::iterator iter2=iter; iter2!=mapStream.end(); ++iter2)
			{
				if (iter2->second == stream) iter2->second = 0;
			}
			delete stream;
		}
		mapStream.clear();
		msInstance = nullptr;*/
	}

	LogManager& LogManager::getInstance()
	{
		if (msInstance == nullptr)
		{
			MYGUI_DBG_BREAK;
			MYGUI_BASE_EXCEPT("Singleton instance LogManager was not created", "MyGUI");
		}
		return *msInstance;
	}

	LogManager* LogManager::getInstancePtr()
	{
		return msInstance;
	}

	/*void LogManager::registerSection(const std::string& _section, const std::string& _source)
	{
	}

	void LogManager::unregisterSection(const std::string& _section, const std::string& _source)
	{
	}*/

	void LogManager::flush()
	{
		for (VectorLogSource::iterator item=mSources.begin(); item!=mSources.end(); ++item)
			(*item)->flush();
	}

	void LogManager::log(const std::string& _section, LogLevel _level, const std::string& _message, const char* _file, int _line)
	{
		time_t ctTime;
		time(&ctTime);
		struct tm *currentTime;
		currentTime = localtime(&ctTime);

		for (VectorLogSource::iterator item=mSources.begin(); item!=mSources.end(); ++item)
			(*item)->log(_section, _level, currentTime, _message, _file, _line);
	}

	void LogManager::close()
	{
		for (VectorLogSource::iterator item=mSources.begin(); item!=mSources.end(); ++item)
			(*item)->close();
	}

	void LogManager::addLogSource(LogSource* _source)
	{
		mSources.push_back(_source);
	}

	void LogManager::createDefaultListeners(const std::string& _logname)
	{
		if (!_logname.empty())
		{
			ConsoleLogListener* console = new ConsoleLogListener();
			FileLogListener* file = new FileLogListener(_logname);
			file->open();

			LogSource* source = new LogSource();
			source->addLogListener(file);
			source->addLogListener(console);

			LogManager::getInstance().addLogSource(source);
		}
	}

	//void LogManager::shutdown()
	//{
		/*if (msInstance != nullptr)
		{
			delete msInstance;
			msInstance = nullptr;
		}*/
	//}

	//void LogManager::initialise()
	//{
		/*if (msInstance == nullptr)
		{
			msInstance = new LogManager();
		}*/
	//}

	/*LogStream& LogManager::out(const std::string& _section, LogLevel _level)
	{
		static LogStream empty;

		if (msInstance == nullptr)
			return empty;

		MapLogStream& mapStream = msInstance->mMapSectionFileName;
		MapLogStream::iterator iter = mapStream.find(_section);
		if (iter == mapStream.end())
			return empty;

		if (_level >= EndLogLevel)
			_level = Info;

		iter->second->start(_section, LevelsName[_level]);

		return *(iter->second);
	}

	void LogManager::registerSection(const std::string& _section, const std::string& _file)
	{
		if (nullptr == msInstance) new LogManager();

		// ищем такую же секцию и удаляем ее
		MapLogStream& mapStream = msInstance->mMapSectionFileName;

		// ищем поток с таким же именем, если нет, то создаем
		LogStream * stream = 0;
		for (MapLogStream::iterator iter=mapStream.begin(); iter!=mapStream.end(); ++iter)
		{
			if (iter->second->getFileName() == _file)
			{
				stream = iter->second;
				break;
			}
		}
		if (0 == stream)
			stream = new LogStream(_file);

		mapStream[_section] = stream;
	}

	void LogManager::unregisterSection(const std::string& _section)
	{
		MapLogStream& mapStream = msInstance->mMapSectionFileName;
		MapLogStream::iterator iter = mapStream.find(_section);
		if (iter == mapStream.end()) return;

		LogStream * stream = iter->second;
		mapStream.erase(iter);

		// если файл еще используеться то удалять не надо
		for (iter=mapStream.begin(); iter!=mapStream.end(); ++iter)
		{
			if (iter->second == stream)
				return;
		}

		delete stream;

		if (mapStream.size() == 0) shutdown();
	}

	const std::string& LogManager::info(const char * _file , int _line )
	{
		std::ostringstream stream;
		stream << separator << _file << separator << _line;

		static std::string ret;
		ret = stream.str();
		return ret;
	}

	const LogStream::LogStreamEnd& LogManager::end()
	{
		return endl;
	}

	void LogManager::setSTDOutputEnabled(bool _enable)
	{
		assert(msInstance);
		msInstance->mSTDOut = _enable;
	}

	bool LogManager::getSTDOutputEnabled()
	{
		assert(msInstance);
		return msInstance->mSTDOut;
	}*/

} // namespace MyGUI
