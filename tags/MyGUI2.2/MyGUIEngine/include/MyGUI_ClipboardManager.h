/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_CLIPBOARD_MANAGER_H__
#define __MYGUI_CLIPBOARD_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	class MYGUI_EXPORT ClipboardManager
	{
		MYGUI_INSTANCE_HEADER(ClipboardManager);

	public:
		void initialise();
		void shutdown();

		/** Set current data in clipboard
			@param _type of data (for example "Text")
			@param _data
		*/
		void SetClipboardData(const std::string& _type, const std::string& _data);
		/** Clear specific type data
			@param _type of data to delete (for example "Text")
		*/
		void ClearClipboardData(const std::string& _type);
		/** Get specific type data
			@param _type of data to get (for example "Text")
		*/
		std::string GetClipboardData(const std::string& _type);

	private:
		MapString mClipboardData;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// дискриптор нашего главного окна
	size_t mHwnd;
	// строка, которую мы положили в буфер обмена винды
	Ogre::DisplayString mPutTextInClipboard;
#endif

	};

}

#endif // __MYGUI_CLIPBOARD_MANAGER_H__
