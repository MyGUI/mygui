/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#include "Precompiled.h"
#include "ResourceItemInfo.h"
#include "MyGUI_ResourceManager.h"

namespace demo
{

	void ResourceItemInfo::deserialize(pugi::xml_node _node, MyGUI::Version _version)
	{
		Base::deserialize(_node, _version);

		for (auto node : _node.children())
		{
			if (std::string_view(node.name()) == "Name")
				mItemName = node.text().as_string();
			else if (std::string_view(node.name()) == "Description")
				mItemDescription = node.text().as_string();
			else if (std::string_view(node.name()) == "Image")
				mItemResourceImage = node.attribute("RefID").value();
		}
	}

	const std::string& ResourceItemInfo::getItemName()
	{
		return mItemName;
	}

	const std::string& ResourceItemInfo::getItemDescription()
	{
		return mItemDescription;
	}

	const std::string& ResourceItemInfo::getItemResourceImage()
	{
		return mItemResourceImage;
	}

} // namespace demo
