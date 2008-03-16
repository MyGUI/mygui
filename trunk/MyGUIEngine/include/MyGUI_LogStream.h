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

	struct _MyGUIExport LogStreamEnd {};

	class LogManager;
	class _MyGUIExport LogStream
	{
		friend class LogManager;

	public:
		inline LogStream& operator<<(const LogStreamEnd& _endl)
		{
			std::cout << std::endl;
			if (mStream.is_open()) {
				mStream << std::endl;
				mStream.close();
			}
			release();

			return *this;
		}

		inline LogStream& operator<<(bool _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline LogStream& operator<<(short _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline LogStream& operator<<(unsigned short _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline LogStream& operator<<(int _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline LogStream& operator<<(long _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline LogStream& operator<<(unsigned long _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline LogStream& operator<<(float _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline LogStream& operator<<(double _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline LogStream& operator<<(long double _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline LogStream& operator<<(size_t _value)
		{
			std::cout << (unsigned int)_value;
			if (mStream.is_open()) mStream << (unsigned int)_value;

			return *this;
		}

		inline LogStream& operator<<(const void * _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline LogStream& operator<<(const char * _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline LogStream& operator<<(const std::string& _value)
		{
			std::cout << _value;
			if (mStream.is_open()) mStream << _value;

			return *this;
		}

		inline const std::string& getFileName() {return mFileName;}

	private:
		LogStream();
		~LogStream();

		LogStream(const std::string& _file);

		void start(const std::string& _section, const std::string& _level);

		inline void lock()
		{
		}

		inline void release()
		{
		}

	private:
		std::ofstream mStream;
		std::string mFileName;

	};

	typedef std::map<std::string, LogStream*> MapLogStream;

} // namespace MyGUI

#endif // __MYGUI_LOG_STREAM_H__
