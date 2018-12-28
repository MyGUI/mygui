/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#ifndef RESOURCE_POINTER_CONTEXT_H_
#define RESOURCE_POINTER_CONTEXT_H_

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

		void deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version) override;

		bool isHighLevel() const;
		std::string getPointer(const std::string& _type);

	private:
		MyGUI::MapString mPointers;
		bool mHighLevel;
	};

}

#endif // RESOURCE_POINTER_CONTEXT_H_
