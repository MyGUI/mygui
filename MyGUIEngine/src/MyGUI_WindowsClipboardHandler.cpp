/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_TextIterator.h"
#include "MyGUI_WindowsClipboardHandler.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32

	#include <windows.h>

namespace MyGUI
{

	HWND g_hWnd = nullptr;

	BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam)
	{
		DWORD dwProcessID = 0;
		GetWindowThreadProcessId(hWnd, &dwProcessID);

		if (dwProcessID != (DWORD)lParam)
			return TRUE;

		if (GetParent(hWnd) == nullptr)
		{
			// Found. hWnd is what we need
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
			// Found. hWnd is what we need
			g_hWnd = hWnd;
			return FALSE;
		}

		return TRUE;
	}

	WindowsClipboardHandler::WindowsClipboardHandler()
	{
	}

	void WindowsClipboardHandler::initialise()
	{
		// get executable path
		char buf[MAX_PATH];
		GetModuleFileName(0, (LPCH)&buf, MAX_PATH);
		// get module instance
		HINSTANCE instance = GetModuleHandle(buf);

		EnumChildWindows(GetDesktopWindow(), (WNDENUMPROC)EnumWindowProc, (LPARAM)instance);
		mHwnd = (size_t)g_hWnd;

		ClipboardManager::getInstance().eventClipboardChanged +=
			newDelegate(this, &WindowsClipboardHandler::handleClipboardChanged);
		ClipboardManager::getInstance().eventClipboardRequested +=
			newDelegate(this, &WindowsClipboardHandler::handleClipboardRequested);
	}

	void WindowsClipboardHandler::shutdown()
	{
		ClipboardManager::getInstance().eventClipboardChanged -=
			newDelegate(this, &WindowsClipboardHandler::handleClipboardChanged);
		ClipboardManager::getInstance().eventClipboardRequested -=
			newDelegate(this, &WindowsClipboardHandler::handleClipboardRequested);
	}

	void WindowsClipboardHandler::handleClipboardChanged(std::string_view _type, std::string_view _data)
	{
		if (_type == "Text")
		{
			mPutTextInClipboard = TextIterator::getOnlyText(UString(_data));
			size_t size = (mPutTextInClipboard.size() + 1) * 2;
			// open clipboard
			if (OpenClipboard((HWND)mHwnd))
			{
				EmptyClipboard();
				HGLOBAL hgBuffer = GlobalAlloc(GMEM_DDESHARE, size);
				wchar_t* chBuffer = hgBuffer ? (wchar_t*)GlobalLock(hgBuffer) : nullptr;
				if (chBuffer)
				{
					memcpy(chBuffer, mPutTextInClipboard.asWStr_c_str(), size);
					GlobalUnlock(hgBuffer);
					// put text in clipboard
					SetClipboardData(CF_UNICODETEXT, hgBuffer);
				}
				CloseClipboard();
			}
		}
	}

	void WindowsClipboardHandler::handleClipboardRequested(std::string_view _type, std::string& _data)
	{
		if (_type == "Text")
		{
			UString buff;
			// open clipboard
			if (OpenClipboard((HWND)mHwnd))
			{
				HANDLE hData = GetClipboardData(CF_UNICODETEXT); // extract text from clipboard
				wchar_t* chBuffer = hData ? (wchar_t*)GlobalLock(hData) : nullptr;
				if (chBuffer)
				{
					buff = chBuffer;
					GlobalUnlock(hData);
				}
				CloseClipboard();
			}
			// if the buffer doesn't contain what we put, take from buffer
			if (mPutTextInClipboard != buff)
			{
				// insert tags if needed
				const UString& text = TextIterator::toTagsString(buff);
				_data = text.asUTF8();
			}
		}
	}

} // namespace MyGUI

#endif // MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
