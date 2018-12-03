/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_FileLogListener.h"
#include "MyGUI_ConsoleLogListener.h"
#include "MyGUI_LevelLogFilter.h"
#include <ctime>

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
		MYGUI_ASSERT(nullptr != getInstancePtr(), "Singleton instance LogManager was not created");
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
		mDefaultSource = new LogSource();

		mConsole = new ConsoleLogListener();
		mConsole->setEnabled(mConsoleEnable);
		mDefaultSource->addLogListener(mConsole);

#ifndef EMSCRIPTEN
		mFile = new FileLogListener();
		mFile->setFileName(_logname);
		mDefaultSource->addLogListener(mFile);
#endif

		mFilter = new LevelLogFilter();
		mFilter->setLoggingLevel(mLevel);
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
