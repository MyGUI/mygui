/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinManager.h"

template <> const char* MyGUI::Singleton<tools::SkinManager>::mClassTypeName("SkinManager");

namespace tools
{

	SkinManager::SkinManager() :
		mItemSelected(nullptr)
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

	SkinItem* SkinManager::createChild(const std::string& _name)
	{
		SkinItem* item = new SkinItem();
		item->setName(_name);

		mChilds.push_back(item);

		return item;
	}

	void SkinManager::destroyChild(SkinItem* _item)
	{
		MYGUI_ASSERT(_item != nullptr, "null reference");

		VectorSkinItem::iterator item = std::find(mChilds.begin(), mChilds.end(), _item);
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

	void SkinManager::destroyChild(const std::string& _name)
	{
		SkinItem* item = getChild(_name);
		destroyChild(item);
	}

	SkinItem* SkinManager::getChild(const std::string& _name)
	{
		for (VectorSkinItem::iterator item=mChilds.begin(); item!=mChilds.end(); ++item)
		{
			if ((*item)->getName() == _name)
				return *item;
		}

		return nullptr;
	}

	void SkinManager::destroyAllChilds()
	{
		while (!mChilds.empty())
			destroyChild(mChilds.back());
	}

	EnumeratorSkinItem SkinManager::getChildsEnumerator()
	{
		return EnumeratorSkinItem(mChilds);
	}

	SkinItem* SkinManager::getItemSelected()
	{
		return mItemSelected;
	}
	
	void SkinManager::setItemSelected(SkinItem* _value)
	{
		mItemSelected = _value;
		eventChangeSelection();
	}

} // namespace tools
