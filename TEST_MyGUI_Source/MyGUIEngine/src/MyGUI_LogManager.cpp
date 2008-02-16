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

	const std::string LogManager::LevelsName[EndLogLevel] = 
	{
		"Info",
		"Warning",
		"Error",
		"Critical"
	};

	const std::string LogManager::General = "General";
	const std::string LogManager::separator = "  |  ";

	LogStreamEnd LogManager::endl;
	LogManager* LogManager::msInstance = 0;

	LogManager::LogManager()
	{
		msInstance = this;
	}

	LogManager::~LogManager()
	{
		MapLogStream & mapStream = msInstance->mMapSectionFileName;
		for (MapLogStream::iterator iter=mapStream.begin(); iter!=mapStream.end(); ++iter) {
			LogStream * stream = iter->second;
			if (stream == 0) continue;

			// ���� ��� ����� ������ � ��������
			for (MapLogStream::iterator iter2=iter; iter2!=mapStream.end(); ++iter2) {
				if (iter2->second == stream) iter2->second = 0;
			}
			delete stream;
		}
		mapStream.clear();
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

	LogStream& LogManager::out(const std::string& _section, LogLevel _level)
	{
		lock();

		static LogStream empty;

		if (0 == msInstance) return empty;

		MapLogStream & mapStream = msInstance->mMapSectionFileName;
		MapLogStream::iterator iter = mapStream.find(_section);
		if (iter == mapStream.end()) return empty;

		if (_level >= EndLogLevel) _level = Info;

		iter->second->start(_section, LevelsName[_level]);
		release();

		return *(iter->second);
	}

	void LogManager::registerSection(const std::string& _section, const std::string& _file)
	{
		if (0 == msInstance) new LogManager();

		// ���� ����� �� ������ � ������� ��
		MapLogStream & mapStream = msInstance->mMapSectionFileName;
		MapLogStream::iterator iter = mapStream.find(_section);
		if (iter != mapStream.end()) {
			delete iter->second;
			mapStream.erase(iter);
		}

		// ���� ����� � ����� �� ������, ���� ���, �� �������
		LogStream * stream = 0;
		for (MapLogStream::iterator iter=mapStream.begin(); iter!=mapStream.end(); ++iter) {
			if (iter->second->getFileName() == _file) {
				stream = iter->second;
				break;
			}
		}
		if (0 == stream) stream = new LogStream(_file);

		mapStream[_section] = stream;
	}

	void LogManager::unregisterSection(const std::string& _section)
	{
		MapLogStream & mapStream = msInstance->mMapSectionFileName;
		MapLogStream::iterator iter = mapStream.find(_section);
		if (iter == mapStream.end()) return;

		LogStream * stream = iter->second;
		mapStream.erase(iter);

		// ���� ���� ��� ������������� �� ������� �� ����
		for (iter=mapStream.begin(); iter!=mapStream.end(); ++iter) {
			if (iter->second == stream) return;
		}

		delete stream;
	}

	const std::string& LogManager::info(const char * _file /* = __FILE__*/, int _line /* = __LINE__*/)
	{
		std::ostringstream stream;
		stream << separator << _file << separator << _line;

		static std::string ret;
		ret = stream.str();
		return ret;
	}

	const LogStreamEnd& LogManager::end()
	{
		return endl;
	}

} // namespace MyGUI
