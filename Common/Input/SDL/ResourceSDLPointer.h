/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/

#ifndef RESOURCE_W32_POINTER_H_
#define RESOURCE_W32_POINTER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IResource.h"

#include <SDL.h>

namespace input
{

	class ResourceSDLPointer : public MyGUI::IResource
	{
		MYGUI_RTTI_DERIVED(ResourceSDLPointer)

	public:
		void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version) override;

		SDL_SystemCursor getPointerType()
		{
			return mCursorType;
		}

	private:
		SDL_SystemCursor mCursorType{SDL_NUM_SYSTEM_CURSORS};
	};

}

#endif // RESOURCE_W32_POINTER_H_
