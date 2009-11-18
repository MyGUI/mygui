/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/

#ifndef __RESOURCE_POINTER_CONTEXT_H__
#define __RESOURCE_POINTER_CONTEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IResource.h"

namespace demo
{

	class ResourcePointerContext :
		public MyGUI::IResource
	{
		MYGUI_RTTI_DERIVED( ResourcePointerContext );

	public:
		ResourcePointerContext() { }
		virtual ~ResourcePointerContext() { }

		virtual void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version);

	};

}

#endif // __RESOURCE_POINTER_CONTEXT_H__
