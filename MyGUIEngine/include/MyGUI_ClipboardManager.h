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

	class _MyGUIExport ClipboardManager
	{
		INSTANCE_HEADER(ClipboardManager);

	public:
		void initialise();
		void shutdown();

		void SetClipboardData(const std::string& _type, const std::string& _data);
		void ClearClipboardData(const std::string& _type);
		std::string GetClipboardData(const std::string& _type);

	private:
		MapString mClipboardData;

	};

}

#endif // __MYGUI_CLIPBOARD_MANAGER_H__
