/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/

#include "MyGUI_Loging.h"
#include "stdio.h"
#include "time.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

namespace MyGUI
{

	void MYGUI_LOG(const std::string & _message)
	{
		const std::string _file("MyGUI.log");

    struct tm *current_time;
    time_t ctTime; time(&ctTime);
    current_time = localtime( &ctTime );

		std::ofstream stream;

		static bool first_run = true;
		if (first_run) {
			stream.open(_file.c_str(), std::ios_base::out);
			stream << " ---------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
			stream << "                                          loging report for : "
				<< std::setw(2) << std::setfill('0') << current_time->tm_mon + 1 << "/"
				<< std::setw(2) << std::setfill('0') << current_time->tm_mday << "/"
				<< std::setw(4) << std::setfill('0') << current_time->tm_year + 1900 << "   "
				<< std::setw(2) << std::setfill('0') << current_time->tm_hour << ":"
				<< std::setw(2) << std::setfill('0') << current_time->tm_min << ":"
				<< std::setw(2) << std::setfill('0') << current_time->tm_sec << std::endl;
			stream << " ---------------------------------------------------------------------------------------------------------------------------------- " << std::endl << std::endl;
			first_run = false;
			stream.close();
		}

		stream.open(_file.c_str(), std::ios_base::app);
		if (stream.is_open()) {

			stream
				<< std::setw(2) << std::setfill('0') << current_time->tm_hour << ":"
				<< std::setw(2) << std::setfill('0') << current_time->tm_min << ":"
				<< std::setw(2) << std::setfill('0') << current_time->tm_sec << "   "
				<< _message << std::endl;

			stream.close();
		}
	}

} // namespace MyGUI
