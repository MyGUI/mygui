#pragma once

namespace MyGUI
{
    #ifdef _DEBUG
	    #define _LOG_NEW _LOG("\r\n")
	    void _LOG( const char *format, ... ); // выводит строку лога в файл
	    #define __ASSERT(mygui_exp) assert(mygui_exp)
    #else
	    #define _LOG
	    #define _LOG_NEW _LOG("\r\n")
    //	#define _OUT
	    #define __ASSERT(mygui_exp)
    #endif
}