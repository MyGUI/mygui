/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinListControl.h"
#include "SkinManager.h"

namespace tools
{

	SkinListControl::SkinListControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SkinListControl.layout", _parent),
		mList(nullptr),
		mCreate(nullptr),
		mRename(nullptr),
		mDelete(nullptr)
	{
		assignWidget(mList, "List");
		assignWidget(mCreate, "Create");
		assignWidget(mRename, "Rename");
		assignWidget(mDelete, "Delete");

		mList->eventListChangePosition += MyGUI::newDelegate(this, &SkinListControl::notifyChangePosition);
		mCreate->eventMouseButtonClick += MyGUI::newDelegate(this, &SkinListControl::notifyCreate);
		mRename->eventMouseButtonClick += MyGUI::newDelegate(this, &SkinListControl::notifyRename);
		mDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &SkinListControl::notifyDelete);
	}

	SkinListControl::~SkinListControl()
	{
		mCreate->eventMouseButtonClick -= MyGUI::newDelegate(this, &SkinListControl::notifyCreate);
		mRename->eventMouseButtonClick -= MyGUI::newDelegate(this, &SkinListControl::notifyRename);
		mDelete->eventMouseButtonClick -= MyGUI::newDelegate(this, &SkinListControl::notifyDelete);
		//mList->eventListChangePosition -= MyGUI::newDelegate(this, &SkinListControl::notifyChangePosition);
	}

	void SkinListControl::notifyChangePosition(MyGUI::List* _sender, size_t _index)
	{
		SkinItem* item = nullptr;

		if (_index != MyGUI::ITEM_NONE)
			item = *mList->getItemDataAt<SkinItem*>(_index);

		SkinManager::getInstance().setItemSelected(item);
	}

	void SkinListControl::notifyCreate(MyGUI::Widget* _sender)
	{
		SkinItem* item = SkinManager::getInstance().createChild("");
		item->setName(MyGUI::utility::toString((int)item));

		UpdateList();
	}

	void SkinListControl::notifyRename(MyGUI::Widget* _sender)
	{
	}

	void SkinListControl::notifyDelete(MyGUI::Widget* _sender)
	{
		SkinItem* item = SkinManager::getInstance().getItemSelected();
		if (item != nullptr)
		{
			SkinManager::getInstance().destroyChild(item);

			UpdateList();
		}
	}

	void SkinListControl::UpdateList()
	{
		mList->setIndexSelected(MyGUI::ITEM_NONE);
		mList->removeAllItems();

		SkinItem* selectedItem = SkinManager::getInstance().getItemSelected();

		EnumeratorSkinItem items = SkinManager::getInstance().getChildsEnumerator();
		while (items.next())
		{
			size_t index = mList->getItemCount();

			SkinItem* item = items.current();
			mList->addItem(item->getName());

			mList->setItemDataAt(index, item);
			if (item == selectedItem)
				mList->setIndexSelected(index);
		}
	}

} // namespace tools
