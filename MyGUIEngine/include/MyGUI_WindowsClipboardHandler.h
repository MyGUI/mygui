/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_WINDOWS_CLIPBOARD_HANDLER_H_
#define MYGUI_WINDOWS_CLIPBOARD_HANDLER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_UString.h"

namespace MyGUI
{

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	class MYGUI_EXPORT WindowsClipboardHandler
	{
	public:
		WindowsClipboardHandler();

		void initialise();
		void shutdown();

		void handleClipboardChanged(const std::string& _type, const std::string& _data);
		void handleClipboardRequested(const std::string& _type, std::string& _data);
	private:
		// дескриптор нашего главного окна
		size_t mHwnd;
		// строка, которую мы положили в буфер обмена винды
		UString mPutTextInClipboard;
	};
#endif // MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32

} // namespace MyGUI

#endif // MYGUI_WINDOWS_CLIPBOARD_HANDLER_H_
