/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#include "ResourcePointerContext.h"

namespace demo
{

	ResourcePointerContext::ResourcePointerContext() :
		mHighLevel(false)
	{
	}

	void ResourcePointerContext::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
	{
		Base::deserialization(_node, _version);

		MyGUI::xml::ElementEnumerator info = _node->getElementEnumerator();
		while (info.next())
		{
			if (info->getName() == "Property")
			{
				const std::string& key = info->findAttribute("key");

				if (key == "Level")
					mHighLevel = info->getContent() == "High";
			}
			else if (info->getName() == "Map")
			{
				MyGUI::xml::ElementEnumerator item = info->getElementEnumerator();
				while (item.next("Item"))
				{
					mPointers[item->findAttribute("name")] = item->getContent();
				}
			}
		}
	}

	std::string ResourcePointerContext::getPointer(const std::string& _type)
	{
		MyGUI::MapString::iterator item = mPointers.find(_type);
		if (item != mPointers.end())
			return item->second;
		return "";
	}

	bool ResourcePointerContext::isHighLevel() const
	{
		return mHighLevel;
	}

}
