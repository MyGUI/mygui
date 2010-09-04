/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinManager.h"

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

	void SkinManager::serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		ItemHolder<SkinItem>::EnumeratorItem items = getChildsEnumerator();
		while (items.next())
		{
			MyGUI::xml::Element* node = _node->createChild("SkinItem");
			items->serialization(node, _version);
		}
	}

	void SkinManager::deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
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
	}

} // namespace tools
