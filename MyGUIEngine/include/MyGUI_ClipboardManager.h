/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_CLIPBOARD_MANAGER_H_
#define MYGUI_CLIPBOARD_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_Types.h"
#include "MyGUI_UString.h"

namespace MyGUI
{

	class WindowsClipboardHandler;

	class MYGUI_EXPORT ClipboardManager :
		public Singleton<ClipboardManager>
	{
	public:
		ClipboardManager();

		void initialise();
		void shutdown();

		/** Set current data in clipboard and trigger eventClipboardChanged. To be used by widgets.
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

		/*events:*/
		/** Event : Clipboard content was changed via setClipboardData.\n
			signature : void method(const std::string& _type, const std::string& _data)\n
			@param _type of data (for example "Text")
			@param _data
		*/
		delegates::CMultiDelegate2<const std::string&, const std::string&> eventClipboardChanged;

		/** Event : The content of the clipboard is being requested via getClipboardData.\n
			Delegates of this event can modify the _data argument in-place to change the data returned by getClipboardData.
			signature : void method(const std::string& _type, std::string& _data)\n
			@param _type of data (for example "Text")
			@param _data 
		*/
		delegates::CMultiDelegate2<const std::string&, std::string&> eventClipboardRequested;

	private:
		MapString mClipboardData;

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		WindowsClipboardHandler* mWindowsClipboardHandler;
#endif

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // MYGUI_CLIPBOARD_MANAGER_H_
