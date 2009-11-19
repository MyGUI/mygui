/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/

#include "ResourceW32Pointer.h"

namespace demo
{

	ResourceW32Pointer::ResourceW32Pointer()
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

				if (key == "Source")
					mPointer = info->getContent();
			}
		}
	}

}
