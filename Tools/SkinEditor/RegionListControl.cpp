/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "RegionListControl.h"
#include "SkinManager.h"

namespace tools
{

	RegionListControl::RegionListControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("RegionListControl.layout", _parent),
		mList(nullptr)
	{
		assignWidget(mList, "List");

		mList->eventListChangePosition += MyGUI::newDelegate(this, &RegionListControl::notifyChangePosition);

		initialiseAdvisor();
	}

	RegionListControl::~RegionListControl()
	{
		shutdownAdvisor();

		mList->eventListChangePosition -= MyGUI::newDelegate(this, &RegionListControl::notifyChangePosition);
	}

	void RegionListControl::notifyChangePosition(MyGUI::List* _sender, size_t _index)
	{
		if (getCurrentSkin() != nullptr)
		{
			RegionItem* item = nullptr;

			if (_index != MyGUI::ITEM_NONE)
				item = *mList->getItemDataAt<RegionItem*>(_index);

			getCurrentSkin()->getRegions().setItemSelected(item);
		}
	}

	void RegionListControl::updateRegionProperty(Property* _sender, const MyGUI::UString& _value)
	{
		if (_sender->getName() == "Visible" || _sender->getName() == "Enabled")
			updateList();
	}

	void RegionListControl::updateRegionProperties()
	{
		updateList();
	}

	void RegionListControl::updateList()
	{
		mList->setIndexSelected(MyGUI::ITEM_NONE);
		mList->removeAllItems();

		if (getCurrentSkin() != nullptr)
		{
			RegionItem* selectedItem = getCurrentSkin()->getRegions().getItemSelected();

			ItemHolder<RegionItem>::EnumeratorItem regions = getCurrentSkin()->getRegions().getChildsEnumerator();
			while (regions.next())
			{
				size_t index = mList->getItemCount();

				RegionItem* item = regions.current();

				if (item->getPropertySet()->getPropertyValue("Visible") != "True"
					|| item->getPropertySet()->getPropertyValue("Enabled") != "True")
						mList->addItem("#808080" + item->getName(), item);
				else
					mList->addItem(item->getName(), item);

				if (item == selectedItem)
					mList->setIndexSelected(index);
			}
		}
	}

} // namespace tools
