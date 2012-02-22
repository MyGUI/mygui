/*!
	@file
	@author		Albert Semenov
	@date		11/2009
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
		MYGUI_RTTI_DERIVED( ResourcePointerContext )

	public:
		ResourcePointerContext();
		virtual ~ResourcePointerContext() { }

		virtual void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version);

		bool isHighLevel() const;
		std::string getPointer(const std::string& _type);

	private:
		MyGUI::MapString mPointers;
		bool mHighLevel;
	};

}

#endif // __RESOURCE_POINTER_CONTEXT_H__
