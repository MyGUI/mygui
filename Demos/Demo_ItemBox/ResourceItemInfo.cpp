/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "ResourceItemInfo.h"
#include "MyGUI_ResourceManager.h"

namespace demo
{

	ResourceItemInfo::ResourceItemInfo(MyGUI::xml::xmlNodeIterator _node) :
		IResource(_node)
	{
		MyGUI::xml::xmlNodeIterator node = _node->getNodeIterator();
		while (node.nextNode()) {
			if (node->getName() == "Name") mItemName = node->getBody();
			else if (node->getName() == "Description") mItemDescription = node->getBody();
			else if (node->getName() == "Image") mItemResourceImage = node->findAttribute("RefID");
		};
	}

} // namespace demo
