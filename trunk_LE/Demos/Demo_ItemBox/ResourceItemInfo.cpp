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

	void ResourceItemInfo::deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version)
	{
		Base::deserialization(_node, _version);

		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Name")
				mItemName = node->getContent();
			else if (node->getName() == "Description")
				mItemDescription = node->getContent();
			else if (node->getName() == "Image")
				mItemResourceImage = node->findAttribute("RefID");
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
