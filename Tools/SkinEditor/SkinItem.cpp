/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinItem.h"

namespace tools
{

	SkinItem::SkinItem() :
		mPropertySet(nullptr),
		mItemSelected(nullptr)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Texture", "Texture");
		mPropertySet->createChild("Coord", "IntCoord", "0 0 32 32");

		createChild("disabled");
		createChild("normal");
		createChild("over");
		createChild("pressed");
		createChild("selected disabled");
		createChild("selected normal");
		createChild("selected over");
		createChild("selected pressed");
	}

	SkinItem::~SkinItem()
	{
		destroyAllChilds();

		delete mPropertySet;
	}

	const MyGUI::UString& SkinItem::getName()
	{
		return mName;
	}

	void SkinItem::setName(const MyGUI::UString& _value)
	{
		mName = _value;
	}

	PropertySet* SkinItem::getPropertySet()
	{
		return mPropertySet;
	}

	StateItem* SkinItem::createChild(const MyGUI::UString& _name)
	{
		StateItem* item = new StateItem();
		item->setName(_name);

		mChilds.push_back(item);

		return item;
	}

	void SkinItem::destroyChild(StateItem* _item)
	{
		MYGUI_ASSERT(_item != nullptr, "null reference");

		VectorStateItem::iterator item = std::find(mChilds.begin(), mChilds.end(), _item);
		if (item != mChilds.end())
		{
			if (*item == mItemSelected)
			{
				mItemSelected = nullptr;
				eventChangeSelection();
			}

			delete *item;
			mChilds.erase(item);
		}
		else
		{
			MYGUI_EXCEPT("item not found");
		}
	}

	void SkinItem::destroyChild(const MyGUI::UString& _name)
	{
		StateItem* item = getChild(_name);
		destroyChild(item);
	}

	void SkinItem::destroyAllChilds()
	{
		while (!mChilds.empty())
			destroyChild(mChilds.back());
	}

	StateItem* SkinItem::getChild(const MyGUI::UString& _name)
	{
		for (VectorStateItem::iterator item=mChilds.begin(); item!=mChilds.end(); ++item)
		{
			if ((*item)->getName() == _name)
				return *item;
		}

		return nullptr;
	}

	EnumeratorStateItem SkinItem::getChildsEnumerator()
	{
		return EnumeratorStateItem(mChilds);
	}

	StateItem* SkinItem::getItemSelected()
	{
		return mItemSelected;
	}

	void SkinItem::setItemSelected(StateItem* _value)
	{
		mItemSelected = _value;
		eventChangeSelection();
	}

} // namespace tools
