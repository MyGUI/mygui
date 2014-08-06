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
#include "MyGUI_WindowsClipboardHandler.h"
#endif

namespace MyGUI
{

	template <> ClipboardManager* Singleton<ClipboardManager>::msInstance = nullptr;
	template <> const char* Singleton<ClipboardManager>::mClassTypeName = "ClipboardManager";

	ClipboardManager::ClipboardManager() :
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		mWindowsClipboardHandler(nullptr),
#endif
		mIsInitialise(false)
	{
	}

	void ClipboardManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		mWindowsClipboardHandler = new WindowsClipboardHandler();
		mWindowsClipboardHandler->initialise();
#endif

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void ClipboardManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		mWindowsClipboardHandler->shutdown();
		delete mWindowsClipboardHandler;
		mWindowsClipboardHandler = nullptr;
#endif

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void ClipboardManager::setClipboardData(const std::string& _type, const std::string& _data)
	{
		mClipboardData[_type] = _data;

		eventClipboardChanged(_type, _data);
	}

	void ClipboardManager::clearClipboardData(const std::string& _type)
	{
		MapString::iterator iter = mClipboardData.find(_type);
		if (iter != mClipboardData.end()) mClipboardData.erase(iter);
	}

	std::string ClipboardManager::getClipboardData(const std::string& _type)
	{
		std::string ret;
		MapString::iterator iter = mClipboardData.find(_type);
		if (iter != mClipboardData.end())
			ret = (*iter).second;

		// Give delegates a chance to fill the clipboard with data
		eventClipboardRequested(_type, ret);
		return ret;
	}

} // namespace MyGUI
