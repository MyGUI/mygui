
#pragma once

#include "stringUtil.h"
#include "stdio.h"
#include "time.h"

namespace widget
{

	#ifdef _DEBUG

		struct file_out
		{
			static void out(const std::string & _value)
			{
				const char * file_name = "gui.log"; // файл лога

				static bool first_run = true;
				if (first_run) { // первый раз
					_unlink( file_name );
					first_run = false;
					// Print a header for the log
					time_t t = time(NULL);
					out("---------------------------------------------------------------\r\n");
					out(toString(" logging file created on ", asctime(localtime(&t)), "\r\n"));
					out("---------------------------------------------------------------\r\n\r\n");
				}

				// Open the log file
				FILE *fp = fopen( file_name, "ab" );
				if ( fp ) {
					fputs( _value.c_str(), fp);
					fclose( fp );
				}

			}
		};

		template< class T1>
		inline void LOG (T1 p1)
		{
			file_out::out(toString(p1) + "\r\n");
		}

		template< class T1,  class T2 >
		inline void LOG (T1 p1, T2 p2)
		{
			file_out::out(toString(p1) + toString(p2) + "\r\n");
		}

		template< class T1,  class T2,  class T3 >
		inline void LOG (T1 p1, T2 p2, T3 p3)
		{
			file_out::out(toString(p1) + toString(p2) + toString(p3) + "\r\n");
		}

		template< class T1,  class T2,  class T3, class T4 >
		inline void LOG (T1 p1, T2 p2, T3 p3, T4 p4)
		{
			file_out::out(toString(p1) + toString(p2) + toString(p3) + toString(p4) + "\r\n");
		}

		template< class T1,  class T2,  class T3, class T4, class T5 >
		inline void LOG (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
		{
			file_out::out(toString(p1) + toString(p2) + toString(p3) + toString(p4) + toString(p5) + "\r\n");
		}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
		inline void LOG (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
		{
			file_out::out(toString(p1) + toString(p2) + toString(p3) + toString(p4) + toString(p5) + toString(p6) + "\r\n");
		}

	#else

		template< class T1>
		inline void LOG (T1 p1) {}

		template< class T1,  class T2 >
		inline void LOG (T1 p1, T2 p2) {}

		template< class T1,  class T2,  class T3 >
		inline void LOG (T1 p1, T2 p2, T3 p3) {}

		template< class T1,  class T2,  class T3, class T4 >
		inline void LOG (T1 p1, T2 p2, T3 p3, T4 p4) {}

		template< class T1,  class T2,  class T3, class T4, class T5 >
		inline void LOG (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) {}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
		inline void LOG (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {}

	#endif

} // namespace widget