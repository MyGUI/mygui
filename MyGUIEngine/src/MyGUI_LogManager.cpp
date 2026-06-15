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

	LogManager::LogManager()
	{
		msInstance = this;
	}

	LogManager::~LogManager()
	{
		flush();
		close();

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
		for (auto& source : mSources)
			source->flush();
	}

	void LogManager::log(
		std::string_view _section,
		LogLevel _level,
		std::string_view _message,
		std::string_view _file,
		int _line)
	{
		time_t ctTime;
		time(&ctTime);
		struct tm* currentTime;
		currentTime = localtime(&ctTime);

		for (auto& source : mSources)
			source->log(_section, _level, currentTime, _message, _file, _line);
	}

	void LogManager::close()
	{
		for (auto& source : mSources)
			source->close();
	}

	void LogManager::addLogSource(LogSource* _source)
	{
		mSources.push_back(_source);
	}

	void LogManager::createDefaultSource(std::string_view _logname)
	{
		mDefaultSource = std::make_unique<LogSource>();

		mConsole = std::make_unique<ConsoleLogListener>();
		mConsole->setEnabled(mConsoleEnable);
		mDefaultSource->addLogListener(mConsole.get());

#ifndef EMSCRIPTEN
		mFile = std::make_unique<FileLogListener>();
		mFile->setFileName(_logname);
		mDefaultSource->addLogListener(mFile.get());
#endif

		mFilter = std::make_unique<LevelLogFilter>();
		mFilter->setLoggingLevel(mLevel);
		mDefaultSource->setLogFilter(mFilter.get());

		mDefaultSource->open();

		LogManager::getInstance().addLogSource(mDefaultSource.get());
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
