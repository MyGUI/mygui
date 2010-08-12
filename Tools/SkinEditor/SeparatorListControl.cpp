/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SeparatorListControl.h"
#include "SkinManager.h"
#include "Binary.h"

namespace tools
{

	enum SeparatorPreset
	{
		SeparatorPresetScale = Binary<0>::value,
		SeparatorPreset9Slice = Binary<1111>::value,
		SeparatorPreset3SliceHorScale = Binary<1100>::value,
		SeparatorPreset3SliceVertScale = Binary<11>::value
	};

	SeparatorListControl::SeparatorListControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SeparatorListControl.layout", _parent),
		mList(nullptr),
		mPresets(nullptr)
	{
		mTypeName = MyGUI::utility::toString((int)this);

		assignWidget(mList, "List");
		assignWidget(mPresets, "Presets");

		fillPresets();

		mList->eventListChangePosition += MyGUI::newDelegate(this, &SeparatorListControl::notifyChangePosition);
		mPresets->eventComboChangePosition += MyGUI::newDelegate(this, &SeparatorListControl::notifyComboChangePosition);

		initialiseAdvisor();
	}

	SeparatorListControl::~SeparatorListControl()
	{
		shutdownAdvisor();

		mPresets->eventComboChangePosition -= MyGUI::newDelegate(this, &SeparatorListControl::notifyComboChangePosition);
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

		if (_owner != mTypeName)
			updatePreset();
	}

	void SeparatorListControl::updateSeparatorProperties()
	{
		updateList();
	}

	void SeparatorListControl::updateSkinProperties()
	{
		updatePreset();
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

	void SeparatorListControl::fillPresets()
	{
		mPresets->removeAllItems();
		mPresets->addItem("One scale region", SeparatorPresetScale);
		mPresets->addItem("9 Regions grid", SeparatorPreset9Slice);
		mPresets->addItem("3 Regions horizontal scale", SeparatorPreset3SliceHorScale);
		mPresets->addItem("3 Regions vertical scale", SeparatorPreset3SliceVertScale);
	}

	void SeparatorListControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		if (getCurrentSkin() == nullptr)
			return;

		if (_index == MyGUI::ITEM_NONE)
			return;

		SeparatorPreset preset = *_sender->getItemDataAt<SeparatorPreset>(_index);

		size_t index = 0;
		ItemHolder<SeparatorItem>::EnumeratorItem separators = getCurrentSkin()->getSeparators().getChildsEnumerator();
		while (separators.next())
		{
			SeparatorItem* item = separators.current();
			MyGUI::UString value = ((preset & (1 << index)) != 0) ? "True" : "False";
			item->getPropertySet()->setPropertyValue("Visible", value, mTypeName);

			++index;
		}

		updateList();
	}

	void SeparatorListControl::updatePreset()
	{
		mPresets->setEnabled(getCurrentSkin() != nullptr);

		if (getCurrentSkin() != nullptr)
		{
			int currentPreset = 0;
			int index = 0;

			ItemHolder<SeparatorItem>::EnumeratorItem separators = getCurrentSkin()->getSeparators().getChildsEnumerator();
			while (separators.next())
			{
				SeparatorItem* item = separators.current();
				bool visible = item->getPropertySet()->getPropertyValue("Visible") == "True";
				if (visible)
					currentPreset |= (1 << index);

				++ index;
			}

			size_t indexSelected = MyGUI::ITEM_NONE;

			size_t count = mPresets->getItemCount();
			for (size_t index=0; index<count; ++index)
			{
				SeparatorPreset preset = *mPresets->getItemDataAt<SeparatorPreset>(index);
				if (preset == currentPreset)
				{
					indexSelected = index;
					break;
				}
			}

			mPresets->setIndexSelected(indexSelected);

			mPresets->setEnabled(true);
		}
		else
		{
			mPresets->setEnabled(false);
		}
	}

} // namespace tools
