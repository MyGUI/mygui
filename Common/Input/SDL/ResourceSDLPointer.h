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

	class ResourceSDLPointer :
		public MyGUI::IResource
	{
		MYGUI_RTTI_DERIVED( ResourceSDLPointer );

	public:
		ResourceSDLPointer();
		virtual ~ResourceSDLPointer() { }

		virtual void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version);

		SDL_SystemCursor getPointerType()
		{
			return mCursorType;
		}

	private:
		SDL_SystemCursor mCursorType;
	};

}

#endif // RESOURCE_W32_POINTER_H_
