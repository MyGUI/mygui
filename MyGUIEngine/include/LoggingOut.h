
//#pragma once

#ifndef _LOGGINGOUT_H_
#define _LOGGINGOUT_H_

#include "Prerequest.h"

#include "stringUtil.h"
#include "stdio.h"
#include "time.h"
#include <sstream>


namespace MyGUI
{

	struct file_out
	{
		static void out(const std::string & _value)
		{
			const char * file_name = "gui.log"; // פאיכ כמדא

	        time_t t = time(NULL);
			struct tm *tme = localtime(&t);

			static bool first_run = true;
			if (first_run) _unlink( file_name );

			// Open the log file
			FILE *fp = fopen( file_name, "ab" );
			if ( fp ) {
				if (first_run) {
					// Print a header for the log
					fprintf(fp, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
					fprintf(fp, "                                          logging report for:  %02d/%02d/%04d %02d:%02d:%02d                                            \r\n",
						tme->tm_mon + 1, tme->tm_mday, tme->tm_year + 1900, tme->tm_hour, tme->tm_min, tme->tm_sec);
					fprintf(fp, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n\r\n");
					first_run = false;
				}
				fprintf(fp, "%02d:%02d:%02d   %s\r\n", tme->tm_hour, tme->tm_min, tme->tm_sec, _value.c_str());
				fclose( fp );
			}

		}
	};

	template< class T1>
	inline void LOG (T1 p1)
	{
		file_out::out(toString(p1));
	}

	template< class T1,  class T2 >
	inline void LOG (T1 p1, T2 p2)
	{
		file_out::out(toString(p1) + toString(p2));
	}

	template< class T1,  class T2,  class T3 >
	inline void LOG (T1 p1, T2 p2, T3 p3)
	{
		file_out::out(toString(p1) + toString(p2) + toString(p3));
	}

	template< class T1,  class T2,  class T3, class T4 >
	inline void LOG (T1 p1, T2 p2, T3 p3, T4 p4)
	{
		file_out::out(toString(p1) + toString(p2) + toString(p3) + toString(p4));
	}

	template< class T1,  class T2,  class T3, class T4, class T5 >
	inline void LOG (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		file_out::out(toString(p1) + toString(p2) + toString(p3) + toString(p4) + toString(p5));
	}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
	inline void LOG (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		file_out::out(toString(p1) + toString(p2) + toString(p3) + toString(p4) + toString(p5) + toString(p6));
	}

	#define LOG_MESSAGE(x) {std::stringstream buf; buf << x; LOG(buf.str());}

} // namespace MyGUI


#endif