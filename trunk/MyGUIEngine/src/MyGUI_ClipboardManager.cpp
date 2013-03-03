/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_TextIterator.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#include <windows.h>
#endif

namespace MyGUI
{

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32

	HWND g_hWnd = NULL;

	BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam)
	{
		DWORD dwProcessID = 0;
		GetWindowThreadProcessId(hWnd, &dwProcessID);

		if (dwProcessID != (DWORD)lParam)
			return TRUE;

		if (GetParent(hWnd) == NULL)
		{
			// Нашли. hWnd - то что надо
			g_hWnd = hWnd;
			return FALSE;
		}

		return TRUE;
	}

	BOOL CALLBACK EnumChildWindowProc(HWND hWnd, LPARAM lParam)
	{
		DWORD dwProcessID = 0;
		GetWindowThreadProcessId(hWnd, &dwProcessID);

		if (dwProcessID != GetCurrentProcessId())
			return TRUE;

		if (GetWindowLongPtr(hWnd, GWLP_HINSTANCE) == lParam)
		{
			// Нашли. hWnd - то что надо
			g_hWnd = hWnd;
			return FALSE;
		}

		return TRUE;
	}

#endif

	template <> ClipboardManager* Singleton<ClipboardManager>::msInstance = nullptr;
	template <> const char* Singleton<ClipboardManager>::mClassTypeName = "ClipboardManager";

	ClipboardManager::ClipboardManager() :
		mIsInitialise(false)
	{
	}

	void ClipboardManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		// берем имя нашего экзешника
		char buf[MAX_PATH];
		GetModuleFileName(0, (LPCH)&buf, MAX_PATH);
		// берем инстанс нашего модуля
		HINSTANCE instance = GetModuleHandle(buf);

		EnumChildWindows(GetDesktopWindow(), (WNDENUMPROC)EnumWindowProc, (LPARAM)instance);
		mHwnd = (size_t)g_hWnd;

#endif

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void ClipboardManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void ClipboardManager::setClipboardData(const std::string& _type, const std::string& _data)
	{
		mClipboardData[_type] = _data;

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		if (_type == "Text")
		{
			mPutTextInClipboard = TextIterator::getOnlyText(UString(_data));
			size_t size = (mPutTextInClipboard.size() + 1) * 2;
			//открываем буфер обмена
			if (OpenClipboard((HWND)mHwnd))
			{
				EmptyClipboard(); //очищаем буфер
				HGLOBAL hgBuffer = GlobalAlloc(GMEM_DDESHARE, size);//выделяем память
				wchar_t* chBuffer = hgBuffer ? (wchar_t*)GlobalLock(hgBuffer) : NULL;
				if (chBuffer)
				{
					memcpy(chBuffer, mPutTextInClipboard.asWStr_c_str(), size);
					GlobalUnlock(hgBuffer);//разблокируем память
					SetClipboardData(CF_UNICODETEXT, hgBuffer);//помещаем текст в буфер обмена
				}
				CloseClipboard(); //закрываем буфер обмена
			}
		}
#endif
	}

	void ClipboardManager::clearClipboardData(const std::string& _type)
	{
		MapString::iterator iter = mClipboardData.find(_type);
		if (iter != mClipboardData.end()) mClipboardData.erase(iter);
	}

	std::string ClipboardManager::getClipboardData(const std::string& _type)
	{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		if (_type == "Text")
		{
			UString buff;
			//открываем буфер обмена
			if (OpenClipboard((HWND)mHwnd))
			{
				HANDLE hData = GetClipboardData(CF_UNICODETEXT);//извлекаем текст из буфера обмена
				wchar_t* chBuffer = hData ? (wchar_t*)GlobalLock(hData) : NULL;
				if (chBuffer)
				{
					buff = chBuffer;
					GlobalUnlock(hData);//разблокируем память
				}
				CloseClipboard();//закрываем буфер обмена
			}
			// если в буфере не то что мы ложили, то берем из буфера
			if (mPutTextInClipboard != buff)
			{
				// вставляем теги, если нуно
				const UString& text = TextIterator::toTagsString(buff);
				return text.asUTF8();
			}

			MapString::iterator iter = mClipboardData.find(_type);
			if (iter != mClipboardData.end()) return (*iter).second;
			return "";
		}
#endif

		MapString::iterator iter = mClipboardData.find(_type);
		if (iter != mClipboardData.end()) return (*iter).second;
		return "";
	}

} // namespace MyGUI
