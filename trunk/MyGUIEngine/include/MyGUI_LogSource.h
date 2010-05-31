/*!
	@file
	@author		Albert Semenov
	@date		04/2010
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
#ifndef __MYGUI_LOG_SOURCE_H__
#define __MYGUI_LOG_SOURCE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ILogFilter.h"
#include "MyGUI_ILogListener.h"
#include <vector>

namespace MyGUI
{
	class MYGUI_EXPORT LogSource
	{
	public:
		LogSource();
		~LogSource();

		// DESCRIBEME
		// ������������� ������ ��� ���������
		void setLogFilter(ILogFilter* _filter);
		// DESCRIBEME
		// ��������� ��������� � ��������
		void addLogListener(ILogListener* _lestener);

		// DESCRIBEME
		// ��������� ���� ����������� open
		void open();
		// DESCRIBEME
		// ��������� ���� ����������� close
		void close();
		// DESCRIBEME
		// ��������� ���� ����������� flush
		void flush();
		// DESCRIBEME
		// �������� ������� ���� ����������� ����� ������
		void log(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line);

	private:
		typedef std::vector<ILogListener*> VectorLogListeners;
		VectorLogListeners mListeners;
		ILogFilter* mFilter;
	};
}

#endif // __MYGUI_LOG_SOURCE_H__
