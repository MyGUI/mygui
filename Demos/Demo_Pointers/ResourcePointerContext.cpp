/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#include "ResourcePointerContext.h"

namespace demo
{

	void ResourcePointerContext::deserialize(pugi::xml_node _node, MyGUI::Version _version)
	{
		Base::deserialize(_node, _version);

		for (auto info : _node.children())
		{
			if (std::string_view(info.name()) == "Property")
			{
				std::string_view key = info.attribute("key").value();

				if (key == "Level")
					mHighLevel = std::string_view(info.text().as_string()) == "High";
			}
			else if (std::string_view(info.name()) == "Map")
			{
				for (auto item : info.children("Item"))
				{
					MyGUI::mapSet(mPointers, item.attribute("name").value(), item.text().as_string());
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
