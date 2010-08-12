/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SeparatorListControl.h"
#include "SkinManager.h"

namespace tools
{

	SeparatorListControl::SeparatorListControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SeparatorListControl.layout", _parent),
		mList(nullptr)
	{
		assignWidget(mList, "List");

		mList->eventListChangePosition += MyGUI::newDelegate(this, &SeparatorListControl::notifyChangePosition);

		initialiseAdvisor();
	}

	SeparatorListControl::~SeparatorListControl()
	{
		shutdownAdvisor();

		mList->eventListChangePosition -= MyGUI::newDelegate(this, &SeparatorListControl::notifyChangePosition);
	}

	void SeparatorListControl::notifyChangePosition(MyGUI::List* _sender, size_t _index)
	{
		if (getCurrentSkin() != nullptr)
		{
			SeparatorItem* item = nullptr;

			if (_index != MyGUI::ITEM_NONE)
				item = *mList->getItemDataAt<SeparatorItem*>(_index);

			getCurrentSkin()->getSeparators().setItemSelected(item);
		}
	}

	void SeparatorListControl::updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Visible")
			updateList();
	}

	void SeparatorListControl::updateSeparatorProperties()
	{
		updateList();
	}

	void SeparatorListControl::updateList()
	{
		mList->setIndexSelected(MyGUI::ITEM_NONE);
		mList->removeAllItems();

		if (getCurrentSkin() != nullptr)
		{
			SeparatorItem* selectedItem = getCurrentSkin()->getSeparators().getItemSelected();

			ItemHolder<SeparatorItem>::EnumeratorItem separators = getCurrentSkin()->getSeparators().getChildsEnumerator();
			while (separators.next())
			{
				size_t index = mList->getItemCount();

				SeparatorItem* item = separators.current();

				if (item->getPropertySet()->getPropertyValue("Visible") != "True")
					mList->addItem("#808080" + item->getName(), item);
				else
					mList->addItem(item->getName(), item);

				if (item == selectedItem)
					mList->setIndexSelected(index);
			}
		}
	}

} // namespace tools
