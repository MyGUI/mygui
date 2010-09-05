/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include <MyGUI.h>
#include "Utility.h"
#include <windows.h>

namespace string_utility
{

	std::string utf16_to_utf8(const std::wstring& _source)
	{
		const wchar_t* srcPtr = _source.c_str();
		int dstSize = WideCharToMultiByte( CP_UTF8, 0, srcPtr, (int)_source.size(), 0, 0, 0, 0 );
		char* dest = new char [ dstSize + 1 ];
		WideCharToMultiByte( CP_UTF8, 0, srcPtr, (int)_source.size(), dest, dstSize, 0, 0 );
		dest[dstSize] = 0;
		std::string ret = dest;
		delete [] dest;
		return ret;
	}

	std::string utf16_to_ansi(const std::wstring& _source)
	{
		const wchar_t* srcPtr = _source.c_str();
		int dstSize = WideCharToMultiByte( CP_ACP, 0, srcPtr, (int)_source.size(), 0, 0, 0, 0 );
		char* dest = new char [ dstSize + 1 ];
		WideCharToMultiByte( CP_ACP, 0, srcPtr, (int)_source.size(), dest, dstSize, 0, 0 );
		dest[dstSize] = 0;
		std::string ret = dest;
		delete [] dest;
		return ret;
	}

	std::wstring utf8_to_utf16(const std::string& _source)
	{
		const char* srcPtr = _source.c_str();
		int tmpSize = MultiByteToWideChar( CP_UTF8, 0, srcPtr, -1, 0, 0 );
		WCHAR* tmpBuff = new WCHAR [ tmpSize + 1 ];
		MultiByteToWideChar( CP_UTF8, 0, srcPtr, -1, tmpBuff, tmpSize );
		std::wstring ret = tmpBuff;
		delete[] tmpBuff;
		return ret;
	}

	std::wstring ansi_to_utf16(const std::string& _source)
	{
		const char* srcPtr = _source.c_str();
		int tmpSize = MultiByteToWideChar( CP_ACP, 0, srcPtr, -1, 0, 0 );
		WCHAR* tmpBuff = new WCHAR [ tmpSize + 1 ];
		MultiByteToWideChar( CP_ACP, 0, srcPtr, -1, tmpBuff, tmpSize );
		std::wstring ret = tmpBuff;
		delete[] tmpBuff;
		return ret;
	}

} // namespace string_utility
