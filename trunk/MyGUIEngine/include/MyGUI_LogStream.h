/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_LOG_STREAM_H__
#define __MYGUI_LOG_STREAM_H__

#include "MyGUI_Prerequest.h"
#include <fstream>
#include <iostream>

namespace MyGUI
{

	struct MYGUI_EXPORT LogStreamEnd { };

	class LogManager;

	class MYGUI_EXPORT LogStream
	{
		friend class LogManager;

	public:
		LogStream& operator<<(const LogStreamEnd& _endl)
		{
			if (getSTDOutputEnabled()) std::cout << std::endl;
			if (mStream.is_open()) {
				mStream << std::endl;
				mStream.close();
			}
			release();

			return *this;
		}

		template <typename T>
		inline LogStream& operator<<(T _value)
		{
			if (getSTDOutputEnabled()) std::cout << _value;
			if (mStream.is_open()) mStream << _value;
			return *this;
		}

		const std::string& getFileName() {return mFileName;}

	private:
		LogStream();
		~LogStream();

		LogStream(const std::string& _file);

		void start(const std::string& _section, const std::string& _level);

		bool getSTDOutputEnabled();

		void lock()
		{
		}

		void release()
		{
		}

	private:
		std::ofstream mStream;
		std::string mFileName;
	};

} // namespace MyGUI

#endif // __MYGUI_LOG_STREAM_H__
