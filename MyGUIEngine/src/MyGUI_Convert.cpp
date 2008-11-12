/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/

#include "MyGUI_Convert.h"

namespace MyGUI
{

	namespace convert
	{

		namespace implement
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			std::wstring win_x_to_wide(const std::string& _source, unsigned int _code)
			{
				const char* source_c = _source.c_str();
				int size_w = ::MultiByteToWideChar( _code, 0, source_c, -1, 0, 0 );
				wchar_t* buff_w = new wchar_t [ size_w + 1 ];
				::MultiByteToWideChar( _code, 0, source_c, -1, buff_w, size_w );
				buff_w[size_w] = 0;
				std::wstring result = buff_w;
				delete[] buff_w;
				return result;
			}

			std::string win_wide_to_x(const std::wstring& _source, unsigned int _code)
			{
				const wchar_t* source_w = _source.c_str();
				int size_c = ::WideCharToMultiByte( _code, 0, source_w, (int)_source.size(), 0, 0, 0, 0 );
				char * buff_c = new char [ size_c + 1 ];
				WideCharToMultiByte( _code, 0, source_w, (int)_source.size(), buff_c, size_c, 0, 0 );
				buff_c[size_c] = 0;
				std::string result = buff_c;
				delete[] buff_c;
				return result;
			}

			std::string win_x_to_x(const std::string& _source, unsigned int _from_code, unsigned int _to_code)
			{
				const char* source_c = _source.c_str();
				int size_w = ::MultiByteToWideChar( _from_code, 0, source_c, -1, 0, 0 );
				wchar_t* buff_w = new wchar_t [ size_w + 1 ];
				::MultiByteToWideChar( _from_code, 0, source_c, -1, buff_w, size_w );
				buff_w[size_w] = 0;

				int size_c = ::WideCharToMultiByte( _to_code, 0, buff_w, size_w, 0, 0, 0, 0 );
				char * buff_c = new char [ size_c + 1 ];
				WideCharToMultiByte( _to_code, 0, buff_w, size_w, buff_c, size_c, 0, 0 );
				buff_c[size_c] = 0;
				std::string result = buff_c;

				delete[] buff_c;
				delete[] buff_w;
				return result;
			}
#endif
		} // namespace implement

		namespace implement
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			std::wstring win_x_to_wide(const std::string& _source, unsigned int _code);
			std::string win_wide_to_x(const std::wstring& _source, unsigned int _code);
			std::string win_x_to_x(const std::string& _source, unsigned int _from_code, unsigned int _to_code);
#endif
		} // namespace implement

		inline std::wstring _MyGUIExport ansi_to_wide(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_wide(_source, CP_ACP);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
            return std::wstring();
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}

		inline std::string _MyGUIExport ansi_to_utf8(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_x(_source, CP_ACP, CP_UTF8);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
            return std::string();
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}

		inline std::wstring _MyGUIExport utf8_to_wide(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_wide(_source, CP_UTF8);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
            return std::wstring();
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}

		inline std::string _MyGUIExport utf8_to_ansi(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_x(_source, CP_UTF8, CP_ACP);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
            return std::string();
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}

		inline std::string _MyGUIExport wide_to_ansi(const std::wstring& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_wide_to_x(_source, CP_ACP);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
            return std::string();
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}

		inline std::string _MyGUIExport wide_to_utf8(const std::wstring& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_wide_to_x(_source, CP_UTF8);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
            return std::string();
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}


	} // namespace convert

} // namespace MyGUI
