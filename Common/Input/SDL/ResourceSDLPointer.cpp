/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/

#include "ResourceSDLPointer.h"

namespace input
{

	ResourceSDLPointer::ResourceSDLPointer() :
		mCursorType(SDL_NUM_SYSTEM_CURSORS)
	{
	}

	void ResourceSDLPointer::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
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
					//std::string path = MyGUI::DataManager::getInstance().getDataPath(info->getContent());
					//mCursorType = (size_t)LoadCursorFromFileA(path.c_str());
				}
				else if (key == "SourceSystem")
				{
					std::string value = info->getContent();
					if (value == "SDL_SYSTEM_CURSOR_ARROW")
						mCursorType = SDL_SYSTEM_CURSOR_ARROW;
					else if (value == "SDL_SYSTEM_CURSOR_IBEAM")
						mCursorType = SDL_SYSTEM_CURSOR_IBEAM;
					else if (value == "SDL_SYSTEM_CURSOR_WAIT")
						mCursorType = SDL_SYSTEM_CURSOR_WAIT;
					else if (value == "SDL_SYSTEM_CURSOR_CROSSHAIR")
						mCursorType = SDL_SYSTEM_CURSOR_CROSSHAIR;
					else if (value == "SDL_SYSTEM_CURSOR_SIZENWSE")
						mCursorType = SDL_SYSTEM_CURSOR_SIZENWSE;
					else if (value == "SDL_SYSTEM_CURSOR_SIZENESW")
						mCursorType = SDL_SYSTEM_CURSOR_SIZENESW;
					else if (value == "SDL_SYSTEM_CURSOR_SIZEWE")
						mCursorType = SDL_SYSTEM_CURSOR_SIZEWE;
					else if (value == "SDL_SYSTEM_CURSOR_SIZENS")
						mCursorType = SDL_SYSTEM_CURSOR_SIZENS;
					else if (value == "SDL_SYSTEM_CURSOR_SIZEALL")
						mCursorType = SDL_SYSTEM_CURSOR_SIZEALL;
					else if (value == "SDL_SYSTEM_CURSOR_NO")
						mCursorType = SDL_SYSTEM_CURSOR_NO;
					else if (value == "SDL_SYSTEM_CURSOR_HAND")
						mCursorType = SDL_SYSTEM_CURSOR_HAND;
				}
			}
		}
	}

}
