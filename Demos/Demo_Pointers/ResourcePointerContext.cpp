/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#include "ResourcePointerContext.h"

namespace demo
{

	void ResourcePointerContext::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
	{
		Base::deserialization(_node, _version);

		MyGUI::xml::ElementEnumerator info = _node->getElementEnumerator();
		while (info.next())
		{
			if (info->getName() == "Property")
			{
				std::string_view key = info->findAttribute("key");

				if (key == "Level")
					mHighLevel = info->getContent() == "High";
			}
			else if (info->getName() == "Map")
			{
				MyGUI::xml::ElementEnumerator item = info->getElementEnumerator();
				while (item.next("Item"))
				{
					MyGUI::mapSet(mPointers, item->findAttribute("name"), item->getContent());
				}
			}
		}
	}

	std::string_view ResourcePointerContext::getPointer(std::string_view _type)
	{
		MyGUI::MapString::iterator item = mPointers.find(_type);
		if (item != mPointers.end())
			return item->second;
		return {};
	}

	bool ResourcePointerContext::isHighLevel() const
	{
		return mHighLevel;
	}

}
