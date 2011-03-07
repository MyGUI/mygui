/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "SkinManager.h"
#include "ExportManager.h"

template <> tools::SkinManager* MyGUI::Singleton<tools::SkinManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::SkinManager>::mClassTypeName("SkinManager");

namespace tools
{

	SkinManager::SkinManager()
	{
	}

	SkinManager::~SkinManager()
	{
	}

	void SkinManager::initialise()
	{
	}

	void SkinManager::shutdown()
	{
		destroyAllChilds();
	}

	void SkinManager::clear()
	{
		destroyAllChilds();
	}

	void SkinManager::_serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		ItemHolder<SkinItem>::EnumeratorItem items = getChildsEnumerator();
		while (items.next())
		{
			MyGUI::xml::Element* node = _node->createChild("SkinItem");
			items->serialization(node, _version);
		}
	}

	/*void SkinManager::_deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		if (getItemSelected() != nullptr)
			setItemSelected(nullptr);

		destroyAllChilds(false);

		MyGUI::xml::ElementEnumerator nodes = _node->getElementEnumerator();
		while (nodes.next("SkinItem"))
		{
			SkinItem* item = createChild(false);
			item->deserialization(nodes.current(), _version);
		}

		eventChangeList();
	}*/

	void SkinManager::deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		if (getItemSelected() != nullptr)
			setItemSelected(nullptr);

		destroyAllChilds(false);

		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("Resource"))
		{
			if (node->findAttribute("type") == "ResourceSkin")
			{
				SkinItem* item = createChild(false);
				item->deserialization2(node.current(), _version);
			}
		}

		eventChangeList();
	}

	void SkinManager::serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		MyGUI::xml::Document doc;
		MyGUI::xml::Element* root = doc.createRoot("Root");

		_serialization(root, MyGUI::Version());

		MyGUI::xml::ElementEnumerator skins = root->getElementEnumerator();
		while (skins.next())
			ExportManager::getInstancePtr()->convertSkin(skins.current(), _node->createChild("Resource"));
	}

} // namespace tools
