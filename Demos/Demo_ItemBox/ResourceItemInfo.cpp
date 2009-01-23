/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "precompiled.h"
#include "ResourceItemInfo.h"
#include "MyGUI_ResourceManager.h"

namespace demo
{

	ResourceItemInfo::ResourceItemInfo(MyGUI::xml::ElementEnumerator _node, MyGUI::Version _version) :
		IResource(_node, _version)
	{
		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next()) {
			if (node->getName() == "Name") mItemName = node->getContent();
			else if (node->getName() == "Description") mItemDescription = node->getContent();
			else if (node->getName() == "Image") mItemResourceImage = node->findAttribute("RefID");
		};
	}

} // namespace demo
