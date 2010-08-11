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
		mTypeName = MyGUI::utility::toString((int)this);

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

	void SeparatorListControl::updateSeparatorProperty(Property* _sender, const MyGUI::UString& _value)
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

			//updateRegionEnabled();
		}
	}

	/*void SeparatorListControl::updateRegionEnabled()
	{
		ItemHolder<RegionItem>::EnumeratorItem regions = getCurrentSkin()->getRegions().getChildsEnumerator();
		while (regions.next())
		{
			bool enabled = true;
			RegionItem* item = regions.current();
			MyGUI::Align separator = item->getSeparator();

			if (separator.isLeft() && isSeparatorVisible(MyGUI::Align::Left))
				enabled = false;
			if (separator.isRight() && isSeparatorVisible(MyGUI::Align::Right))
				enabled = false;
			if (separator.isTop() && isSeparatorVisible(MyGUI::Align::Top))
				enabled = false;
			if (separator.isBottom() && isSeparatorVisible(MyGUI::Align::Bottom))
				enabled = false;

			item->getPropertySet()->setPropertyValue("Enabled", enabled ? "True" : "False", mTypeName);
		}
	}

	bool SeparatorListControl::isSeparatorVisible(MyGUI::Align _value)
	{
		ItemHolder<SeparatorItem>::EnumeratorItem separators = getCurrentSkin()->getSeparators().getChildsEnumerator();
		while (separators.next())
		{
			SeparatorItem* item = separators.current();

			if (item->getCorner() == _value)
				return item->getPropertySet()->getPropertyValue("Visible") == "True";
		}

		return true;
	}*/

} // namespace tools
