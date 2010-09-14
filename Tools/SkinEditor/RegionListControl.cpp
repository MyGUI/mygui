/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "RegionListControl.h"
#include "SkinManager.h"
#include "Localise.h"

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

	void RegionListControl::updateRegionProperty(Property* _sender, const MyGUI::UString& _owner)
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

				bool visible = item->getPropertySet()->getPropertyValue("Visible") == "True";
				bool enabled = item->getPropertySet()->getPropertyValue("Enabled") == "True";
				if (!visible || !enabled)
					mList->addItem(replaceTags("ColourDisabled") + item->getName(), item);
				else
					mList->addItem(item->getName(), item);

				if (item == selectedItem)
					mList->setIndexSelected(index);
			}
		}
	}

	void RegionListControl::updateSeparatorProperties()
	{
		updateRegionEnabled();
		updateRegionPosition();
	}

	void RegionListControl::updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Visible" || _sender->getName() == "Offset" )
		{
			updateRegionEnabled();
			updateRegionPosition();
		}
	}

	void RegionListControl::updateSkinProperties()
	{
		updateCoord();
	}

	void RegionListControl::updateSkinProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Coord")
			updateCoord();
	}

	void RegionListControl::updateCoord()
	{
		MyGUI::UString value;

		if (getCurrentSkin() != nullptr)
			value = getCurrentSkin()->getPropertySet()->getPropertyValue("Coord");

		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(value, coord.left, coord.top, coord.width, coord.height))
		{
			mCoordValue = coord;
			updateRegionPosition();
		}
	}

	void RegionListControl::updateRegionEnabled()
	{
		if (getCurrentSkin() != nullptr)
		{
			ItemHolder<RegionItem>::EnumeratorItem regions = getCurrentSkin()->getRegions().getChildsEnumerator();
			while (regions.next())
			{
				bool enabled = true;
				RegionItem* item = regions.current();

				MyGUI::Align separator = item->getSeparator();
				if (separator.isLeft() && !isSeparatorVisible(MyGUI::Align::Left))
					enabled = false;
				if (separator.isRight() && !isSeparatorVisible(MyGUI::Align::Right))
					enabled = false;
				if (separator.isTop() && !isSeparatorVisible(MyGUI::Align::Top))
					enabled = false;
				if (separator.isBottom() && !isSeparatorVisible(MyGUI::Align::Bottom))
					enabled = false;

				MyGUI::UString value = enabled ? "True" : "False";
				if (item->getPropertySet()->getPropertyValue("Enabled") != value)
					item->getPropertySet()->setPropertyValue("Enabled", value, mTypeName);
			}

			updateList();
		}
	}

	void RegionListControl::updateRegionPosition()
	{
		if (getCurrentSkin() == nullptr)
			return;

		bool leftVisible = isSeparatorVisible(MyGUI::Align::Left);
		bool rightVisible = isSeparatorVisible(MyGUI::Align::Right);
		bool topVisible = isSeparatorVisible(MyGUI::Align::Top);
		bool bottomVisible = isSeparatorVisible(MyGUI::Align::Bottom);

		int leftPosition = leftVisible ? getSeparatorPosition(MyGUI::Align::Left) : 0;
		int rightPosition = rightVisible ? getSeparatorPosition(MyGUI::Align::Right) : 0;
		int topPosition = topVisible ? getSeparatorPosition(MyGUI::Align::Top) : 0;
		int bottomPosition = bottomVisible ? getSeparatorPosition(MyGUI::Align::Bottom) : 0;

		RegionItem* item = nullptr;

		item = getRegion(MyGUI::Align::Left | MyGUI::Align::Top);
		if (item != nullptr)
		{
			MyGUI::IntCoord coord(0, 0, leftPosition, topPosition);
			item->getPropertySet()->setPropertyValue("Position", coord.print(), mTypeName);
		}

		item = getRegion(MyGUI::Align::Right | MyGUI::Align::Top);
		if (item != nullptr)
		{
			MyGUI::IntCoord coord(mCoordValue.width - rightPosition, 0, rightPosition, topPosition);
			item->getPropertySet()->setPropertyValue("Position", coord.print(), mTypeName);
		}

		item = getRegion(MyGUI::Align::Right | MyGUI::Align::Bottom);
		if (item != nullptr)
		{
			MyGUI::IntCoord coord(mCoordValue.width - rightPosition, mCoordValue.height - bottomPosition, rightPosition, bottomPosition);
			item->getPropertySet()->setPropertyValue("Position", coord.print(), mTypeName);
		}

		item = getRegion(MyGUI::Align::Left | MyGUI::Align::Bottom);
		if (item != nullptr)
		{
			MyGUI::IntCoord coord(0, mCoordValue.height - bottomPosition, leftPosition, bottomPosition);
			item->getPropertySet()->setPropertyValue("Position", coord.print(), mTypeName);
		}

		item = getRegion(MyGUI::Align::Left);
		if (item != nullptr)
		{
			MyGUI::IntCoord coord(0, topPosition, leftPosition, mCoordValue.height - topPosition - bottomPosition);
			item->getPropertySet()->setPropertyValue("Position", coord.print(), mTypeName);
		}

		item = getRegion(MyGUI::Align::Top);
		if (item != nullptr)
		{
			MyGUI::IntCoord coord(leftPosition, 0, mCoordValue.width - rightPosition - leftPosition, topPosition);
			item->getPropertySet()->setPropertyValue("Position", coord.print(), mTypeName);
		}

		item = getRegion(MyGUI::Align::Right);
		if (item != nullptr)
		{
			MyGUI::IntCoord coord(mCoordValue.width - rightPosition, topPosition, rightPosition, mCoordValue.height - topPosition - bottomPosition);
			item->getPropertySet()->setPropertyValue("Position", coord.print(), mTypeName);
		}

		item = getRegion(MyGUI::Align::Bottom);
		if (item != nullptr)
		{
			MyGUI::IntCoord coord(leftPosition, mCoordValue.height - bottomPosition, mCoordValue.width - rightPosition - leftPosition, bottomPosition);
			item->getPropertySet()->setPropertyValue("Position", coord.print(), mTypeName);
		}

		item = getRegion(MyGUI::Align::Center);
		if (item != nullptr)
		{
			MyGUI::IntCoord coord(leftPosition, topPosition, mCoordValue.width - rightPosition - leftPosition, mCoordValue.height - bottomPosition - topPosition);
			item->getPropertySet()->setPropertyValue("Position", coord.print(), mTypeName);
		}
	}

	bool RegionListControl::isSeparatorVisible(MyGUI::Align _value)
	{
		ItemHolder<SeparatorItem>::EnumeratorItem separators = getCurrentSkin()->getSeparators().getChildsEnumerator();
		while (separators.next())
		{
			SeparatorItem* item = separators.current();

			if (item->getCorner() == _value)
				return item->getPropertySet()->getPropertyValue("Visible") == "True";
		}

		return true;
	}

	int RegionListControl::getSeparatorPosition(MyGUI::Align _value)
	{
		ItemHolder<SeparatorItem>::EnumeratorItem separators = getCurrentSkin()->getSeparators().getChildsEnumerator();
		while (separators.next())
		{
			SeparatorItem* item = separators.current();

			if (item->getCorner() == _value)
				return MyGUI::utility::parseInt(item->getPropertySet()->getPropertyValue("Offset"));
		}

		return 0;
	}

	RegionItem* RegionListControl::getRegion(MyGUI::Align _align)
	{
		ItemHolder<RegionItem>::EnumeratorItem regions = getCurrentSkin()->getRegions().getChildsEnumerator();
		while (regions.next())
		{
			RegionItem* item = regions.current();
			if (item->getSeparator() == _align)
				return item;
		}
		return nullptr;
	}

} // namespace tools
