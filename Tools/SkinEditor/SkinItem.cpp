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
		mStateSelected(nullptr)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Texture", "Texture");
		mPropertySet->createChild("Coord", "IntCoord", "0 0 32 32");

		createState("disabled");
		createState("normal");
		createState("over");
		createState("pressed");
		createState("selected disabled");
		createState("selected normal");
		createState("selected over");
		createState("selected pressed");
	}

	SkinItem::~SkinItem()
	{
		destroyAllStates();

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

	StateItem* SkinItem::createState(const MyGUI::UString& _name)
	{
		StateItem* item = new StateItem();
		item->setName(_name);

		mStates.push_back(item);

		return item;
	}

	void SkinItem::destroyState(StateItem* _item)
	{
		MYGUI_ASSERT(_item != nullptr, "null reference");

		VectorStateItem::iterator item = std::find(mStates.begin(), mStates.end(), _item);
		if (item != mStates.end())
		{
			if (*item == mStateSelected)
			{
				mStateSelected = nullptr;
				eventStateChangeSelection();
			}

			delete *item;
			mStates.erase(item);
		}
		else
		{
			MYGUI_EXCEPT("item not found");
		}
	}

	void SkinItem::destroyState(const MyGUI::UString& _name)
	{
		StateItem* item = getState(_name);
		destroyState(item);
	}

	void SkinItem::destroyAllStates()
	{
		while (!mStates.empty())
			destroyState(mStates.back());
	}

	StateItem* SkinItem::getState(const MyGUI::UString& _name)
	{
		for (VectorStateItem::iterator item=mStates.begin(); item!=mStates.end(); ++item)
		{
			if ((*item)->getName() == _name)
				return *item;
		}

		return nullptr;
	}

	EnumeratorStateItem SkinItem::getStatesEnumerator()
	{
		return EnumeratorStateItem(mStates);
	}

	StateItem* SkinItem::getStateSelected()
	{
		return mStateSelected;
	}

	void SkinItem::setStateSelected(StateItem* _value)
	{
		mStateSelected = _value;
		eventStateChangeSelection();
	}

} // namespace tools
