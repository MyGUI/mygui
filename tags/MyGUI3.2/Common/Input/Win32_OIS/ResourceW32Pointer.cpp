/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/

#include "ResourceW32Pointer.h"
#include <windows.h>

namespace input
{

	ResourceW32Pointer::ResourceW32Pointer() :
		mHandle(0)
	{
	}

	void ResourceW32Pointer::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
	{
		Base::deserialization(_node, _version);

		MyGUI::xml::ElementEnumerator info = _node->getElementEnumerator();
		while (info.next())
		{
			if (info->getName() == "Property")
			{
				const std::string& key = info->findAttribute("key");

				if (key == "SourceFile")
				{
					std::string path = MyGUI::DataManager::getInstance().getDataPath(info->getContent());
					mHandle = (size_t)LoadCursorFromFileA(path.c_str());
				}
				else if (key == "SourceSystem")
				{
					std::string value = info->getContent();
					if (value == "IDC_ARROW")
						mHandle = (size_t)::LoadCursor(NULL, IDC_ARROW);
					else if (value == "IDC_IBEAM")
						mHandle = (size_t)::LoadCursor(NULL, IDC_IBEAM);
					else if (value == "IDC_WAIT")
						mHandle = (size_t)::LoadCursor(NULL, IDC_WAIT);
					else if (value == "IDC_CROSS")
						mHandle = (size_t)::LoadCursor(NULL, IDC_CROSS);
					else if (value == "IDC_UPARROW")
						mHandle = (size_t)::LoadCursor(NULL, IDC_UPARROW);
					else if (value == "IDC_SIZE")
						mHandle = (size_t)::LoadCursor(NULL, IDC_SIZE);
					else if (value == "IDC_ICON")
						mHandle = (size_t)::LoadCursor(NULL, IDC_ICON);
					else if (value == "IDC_SIZENWSE")
						mHandle = (size_t)::LoadCursor(NULL, IDC_SIZENWSE);
					else if (value == "IDC_SIZENESW")
						mHandle = (size_t)::LoadCursor(NULL, IDC_SIZENESW);
					else if (value == "IDC_SIZEWE")
						mHandle = (size_t)::LoadCursor(NULL, IDC_SIZEWE);
					else if (value == "IDC_SIZENS")
						mHandle = (size_t)::LoadCursor(NULL, IDC_SIZENS);
					else if (value == "IDC_SIZEALL")
						mHandle = (size_t)::LoadCursor(NULL, IDC_SIZEALL);
					else if (value == "IDC_NO")
						mHandle = (size_t)::LoadCursor(NULL, IDC_NO);
					else if (value == "IDC_HAND")
						mHandle = (size_t)::LoadCursor(NULL, IDC_HAND);
					else if (value == "IDC_APPSTARTING")
						mHandle = (size_t)::LoadCursor(NULL, IDC_APPSTARTING);
					else if (value == "IDC_HELP")
						mHandle = (size_t)::LoadCursor(NULL, IDC_HELP);
				}
			}
		}
	}

}
