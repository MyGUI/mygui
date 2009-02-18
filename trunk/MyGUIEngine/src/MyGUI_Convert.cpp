/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*//*
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

#include "MyGUI_Precompiled.h"
#include "MyGUI_Convert.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#include "windows.h"
#endif

namespace MyGUI
{

	namespace convert
	{

		namespace implement
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32

			template <typename T, size_t MAX = 2048>
			class AllocHolder
			{
			public:
				AllocHolder(size_t _len)
				{
					if (m_len < _len) {
						m_len = _len * 2;
						delete[] m_buff;
						m_buff = new T[m_len];
					}
				}
				~AllocHolder()
				{
					if (m_len > MAX) Clear();
				}

				T * GetBuff() { return m_buff; }
				size_t GetSize() { return m_len; }

				void Clear()
				{
					delete[] m_buff;
					m_buff = 0;
					m_len = 0;
				}

			private:
				AllocHolder() { }
				static T* m_buff;
				static size_t m_len;
			};
			template<> char* AllocHolder<char>::m_buff = 0;
			template<> size_t AllocHolder<char>::m_len = 0;
			template<> wchar_t* AllocHolder<wchar_t>::m_buff = 0;
			template<> size_t AllocHolder<wchar_t>::m_len = 0;

			typedef AllocHolder<char> AllocHolderChar;
			typedef AllocHolder<wchar_t> AllocHolderWide;


			// после шутдауна удаляем указатели
			struct Terminator
			{
				~Terminator()
				{
					AllocHolderWide holder_w(1);
					holder_w.Clear();
					AllocHolderChar holder_c(1);
					holder_c.Clear();
				}
			};
			Terminator terminator;

			std::wstring win_x_to_wide(const std::string& _source, unsigned int _code)
			{
				const char* source_c = _source.c_str();
				int size_w = ::MultiByteToWideChar( _code, 0, source_c, -1, 0, 0 );
				AllocHolderWide holder_w(size_w + 1);
				wchar_t* buff_w = holder_w.GetBuff();
				::MultiByteToWideChar( _code, 0, source_c, -1, buff_w, size_w );
				buff_w[size_w] = 0;
				std::wstring result = buff_w;
				return result;
			}

			std::string win_wide_to_x(const std::wstring& _source, unsigned int _code)
			{
				const wchar_t* source_w = _source.c_str();
				int size_c = ::WideCharToMultiByte( _code, 0, source_w, (int)_source.size(), 0, 0, 0, 0 );
				AllocHolderChar holder_c(size_c + 1);
				char * buff_c = holder_c.GetBuff();
				WideCharToMultiByte( _code, 0, source_w, (int)_source.size(), buff_c, size_c, 0, 0 );
				buff_c[size_c] = 0;
				std::string result = buff_c;
				return result;
			}

			std::string win_x_to_x(const std::string& _source, unsigned int _from_code, unsigned int _to_code)
			{
				const char* source_c = _source.c_str();
				int size_w = ::MultiByteToWideChar( _from_code, 0, source_c, -1, 0, 0 );
				AllocHolderWide holder_w(size_w + 1);
				wchar_t* buff_w = holder_w.GetBuff();
				::MultiByteToWideChar( _from_code, 0, source_c, -1, buff_w, size_w );
				buff_w[size_w] = 0;
				int size_c = ::WideCharToMultiByte( _to_code, 0, buff_w, size_w, 0, 0, 0, 0 );
				AllocHolderChar holder_c(size_c + 1);
				char * buff_c = holder_c.GetBuff();
				WideCharToMultiByte( _to_code, 0, buff_w, size_w, buff_c, size_c, 0, 0 );
				buff_c[size_c] = 0;
				std::string result = buff_c;
				return result;
			}

#endif // MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32

		} // namespace implement


		std::wstring ansi_to_wide(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_wide(_source, CP_ACP);
#else
			return std::wstring();
#endif
		}

		std::string ansi_to_utf8(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_x(_source, CP_ACP, CP_UTF8);
#else
			return _source;
#endif
		}

		std::string ansi_to_oem(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_x(_source, CP_ACP, CP_OEMCP);
#else
			return _source;
#endif
		}

		std::wstring utf8_to_wide(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_wide(_source, CP_UTF8);
#else
			return std::wstring();
#endif
		}

		std::string utf8_to_ansi(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_x(_source, CP_UTF8, CP_ACP);
#else
			return _source;
#endif
		}

		std::string utf8_to_oem(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_x(_source, CP_UTF8, CP_OEMCP);
#else
			return _source;
#endif
		}

		std::string wide_to_ansi(const std::wstring& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_wide_to_x(_source, CP_ACP);
#else
			return std::string();
#endif
		}

		std::string wide_to_utf8(const std::wstring& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_wide_to_x(_source, CP_UTF8);
#else
			return std::string();
#endif
		}

		std::string wide_to_oem(const std::wstring& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_wide_to_x(_source, CP_OEMCP);
#else
			return std::string();
#endif
		}

	} // namespace convert

} // namespace MyGUI
