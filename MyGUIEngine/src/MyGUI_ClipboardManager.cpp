/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_ClipboardManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(ClipboardManager);

	void ClipboardManager::SetClipboardData(const std::string& _type, const std::string& _data)
	{
		mClipboardData[_type] = _data;
	}

	void ClipboardManager::ClearClipboardData(const std::string& _type)
	{
		MapString::iterator iter = mClipboardData.find(_type);
		if (iter != mClipboardData.end()) mClipboardData.erase(iter);
	}

	std::string ClipboardManager::GetClipboardData(const std::string& _type)
	{
		MapString::iterator iter = mClipboardData.find(_type);
		if (iter != mClipboardData.end()) return (*iter).second;
		return "";
	}

} // namespace MyGUI
