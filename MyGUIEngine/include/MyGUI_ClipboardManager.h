/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_CLIPBOARD_MANAGER_H__
#define __MYGUI_CLIPBOARD_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_Types.h"
#include "MyGUI_UString.h"

namespace MyGUI
{

	class MYGUI_EXPORT ClipboardManager :
		public Singleton<ClipboardManager>
	{
	public:
		ClipboardManager();

		void initialise();
		void shutdown();

		/** Set current data in clipboard
			@param _type of data (for example "Text")
			@param _data
		*/
		void setClipboardData(const std::string& _type, const std::string& _data);
		/** Clear specific type data
			@param _type of data to delete (for example "Text")
		*/
		void clearClipboardData(const std::string& _type);
		/** Get specific type data
			@param _type of data to get (for example "Text")
		*/
		std::string getClipboardData(const std::string& _type);

	private:
		MapString mClipboardData;

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		// дескриптор нашего главного окна
		size_t mHwnd;
		// строка, которую мы положили в буфер обмена винды
		UString mPutTextInClipboard;
#endif

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_CLIPBOARD_MANAGER_H__
