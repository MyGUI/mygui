#pragma warning (disable: 4996) //yes, functions are deprecated, but we'll move on with our lives

#include "MyGUI_Logging.h"
#include "stdio.h"
#include "stdarg.h"
#include "time.h"

namespace MyGUI
{
#ifdef _DEBUG
	void _LOG( const char *format, ... ) // выводит строку лога в файл
    {
		const char * logFileMyGUI = "MyGUI.log"; // файл лога
        char buffer[2048]; // The buffer

		va_list ap;
        va_start( ap, format );
        vsprintf( buffer, format, ap );
        va_end( ap );

		static bool bIsFirstRun = true;
		if (bIsFirstRun) { // первый раз
            _unlink( logFileMyGUI );
			bIsFirstRun = false;
			// Print a header for the log
			time_t t = time(NULL);
			_LOG("--------------------------------------------------------------------------------");
			_LOG(" %s - MyGUI logging file created on %s", logFileMyGUI, asctime(localtime(&t)));
			_LOG("--------------------------------------------------------------------------------\r\n");
		}
        // Open the log file
        FILE *fp = fopen( logFileMyGUI, "ab" );
        if ( !fp ) return;
        // Spit out the data to the log
        fprintf( fp, "%s\r\n", buffer );
        fclose( fp );
    };
#endif

}