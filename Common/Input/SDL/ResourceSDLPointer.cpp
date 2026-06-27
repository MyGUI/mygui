/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/

#include "ResourceSDLPointer.h"

namespace input
{

	void ResourceSDLPointer::deserialize(pugi::xml_node _node, MyGUI::Version _version)
	{
		Base::deserialize(_node, _version);

		for (auto info : _node.children())
		{
			if (std::string_view(info.name()) == "Property")
			{
				std::string_view key = info.attribute("key").value();

				if (key == "SourceFile")
				{
					//std::string path = MyGUI::DataManager::getInstance().getDataPath(info->getContent());
					//mCursorType = (size_t)LoadCursorFromFileA(path.c_str());
				}
				else if (key == "SourceSystem")
				{
					const std::string_view value = info.text().as_string();
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
