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

namespace input
{

	class ResourceW32Pointer :
		public MyGUI::IResource
	{
		MYGUI_RTTI_DERIVED( ResourceW32Pointer );

	public:
		ResourceW32Pointer();
		virtual ~ResourceW32Pointer() { }

		virtual void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version);

		size_t getPointerHandle()
		{
			return mHandle;
		}

	private:
		size_t mHandle;
	};

}

#endif // RESOURCE_W32_POINTER_H_
