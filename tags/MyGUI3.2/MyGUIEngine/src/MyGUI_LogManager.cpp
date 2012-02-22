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
#include "MyGUI_LevelLogFilter.h"
#include "MyGUI_LogSource.h"
#include <time.h>

namespace MyGUI
{

	LogManager* LogManager::msInstance = nullptr;

	LogManager::LogManager() :
		mConsole(nullptr),
		mFile(nullptr),
		mFilter(nullptr),
		mDefaultSource(nullptr),
		mLevel(LogLevel::Info),
		mConsoleEnable(true)
	{
		msInstance = this;
	}

	LogManager::~LogManager()
	{
		flush();
		close();

		delete mDefaultSource;
		mDefaultSource = nullptr;
		delete mConsole;
		mConsole = nullptr;
		delete mFile;
		mFile = nullptr;
		delete mFilter;
		mFilter = nullptr;

		msInstance = nullptr;
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

	void LogManager::flush()
	{
		for (VectorLogSource::iterator item = mSources.begin(); item != mSources.end(); ++item)
			(*item)->flush();
	}

	void LogManager::log(const std::string& _section, LogLevel _level, const std::string& _message, const char* _file, int _line)
	{
		time_t ctTime;
		time(&ctTime);
		struct tm* currentTime;
		currentTime = localtime(&ctTime);

		for (VectorLogSource::iterator item = mSources.begin(); item != mSources.end(); ++item)
			(*item)->log(_section, _level, currentTime, _message, _file, _line);
	}

	void LogManager::close()
	{
		for (VectorLogSource::iterator item = mSources.begin(); item != mSources.end(); ++item)
			(*item)->close();
	}

	void LogManager::addLogSource(LogSource* _source)
	{
		mSources.push_back(_source);
	}

	void LogManager::createDefaultSource(const std::string& _logname)
	{
		mConsole = new ConsoleLogListener();
		mFile = new FileLogListener();
		mFilter = new LevelLogFilter();

		mFile->setFileName(_logname);
		mConsole->setEnabled(mConsoleEnable);
		mFilter->setLoggingLevel(mLevel);

		mDefaultSource = new LogSource();
		mDefaultSource->addLogListener(mFile);
		mDefaultSource->addLogListener(mConsole);
		mDefaultSource->setLogFilter(mFilter);

		mDefaultSource->open();

		LogManager::getInstance().addLogSource(mDefaultSource);
	}

	void LogManager::setSTDOutputEnabled(bool _value)
	{
		mConsoleEnable = _value;

		if (mConsole != nullptr)
			mConsole->setEnabled(_value);
	}

	bool LogManager::getSTDOutputEnabled() const
	{
		return mConsoleEnable;
	}

	void LogManager::setLoggingLevel(LogLevel _value)
	{
		mLevel = _value;

		if (mFilter != nullptr)
			mFilter->setLoggingLevel(_value);
	}

	LogLevel LogManager::getLoggingLevel() const
	{
		return mLevel;
	}

} // namespace MyGUI
