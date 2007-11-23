#ifndef __CLIPBOARD_MANAGER_H__
#define __CLIPBOARD_MANAGER_H__

#include "Prerequest.h"
#include "Instance.h"
#include "Common.h"

namespace MyGUI
{

	class _MyGUIExport ClipboardManager
	{
		INSTANCE_HEADER(ClipboardManager);

	public:
		typedef std::map<std::string, std::string> MapString;

		void SetClipboardData(const std::string& _type, const std::string& _data);
		void ClearClipboardData(const std::string& _type);
		std::string GetClipboardData(const std::string& _type);

	private:
		MapString mClipboardData;

	};

}

#endif // __CLIPBOARD_MANAGER_H__