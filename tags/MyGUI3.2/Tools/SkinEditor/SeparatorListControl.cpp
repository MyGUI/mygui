/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "SeparatorListControl.h"
#include "SkinManager.h"
#include "Binary.h"
#include "Localise.h"

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
		mTypeName = MyGUI::utility::toString((size_t)this);

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

	void SeparatorListControl::notifyChangePosition(MyGUI::ListBox* _sender, size_t _index)
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
		if (getCurrentSkin() != nullptr)
		{
			SeparatorItem* selectedItem = getCurrentSkin()->getSeparators().getItemSelected();
			size_t selectedIndex = MyGUI::ITEM_NONE;

			size_t index = 0;
			ItemHolder<SeparatorItem>::EnumeratorItem separators = getCurrentSkin()->getSeparators().getChildsEnumerator();
			while (separators.next())
			{
				SeparatorItem* item = separators.current();

				MyGUI::UString name;
				if (item->getPropertySet()->getPropertyValue("Visible") != "True")
					name = replaceTags("ColourDisabled") + item->getName();
				else
					name = item->getName();

				if (index < mList->getItemCount())
				{
					mList->setItemNameAt(index, name);
					mList->setItemDataAt(index, item);
				}
				else
				{
					mList->addItem(name, item);
				}

				if (item == selectedItem)
					selectedIndex = index;

				index ++;
			}

			while (index < mList->getItemCount())
				mList->removeItemAt(mList->getItemCount() - 1);

			mList->setIndexSelected(selectedIndex);
		}
	}

	void SeparatorListControl::fillPresets()
	{
		mPresets->removeAllItems();
		mPresets->addItem(replaceTags("PresetRegionOneScale"), SeparatorPresetScale);
		mPresets->addItem(replaceTags("PresetRegion9Grid"), SeparatorPreset9Slice);
		mPresets->addItem(replaceTags("PresetRegion3Hor"), SeparatorPreset3SliceHorScale);
		mPresets->addItem(replaceTags("PresetRegion3Vert"), SeparatorPreset3SliceVertScale);

		mPresets->beginToItemFirst();
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

		// для обновления пропертей
		getCurrentSkin()->getSeparators().setItemSelected(nullptr);

		updateList();
	}

	void SeparatorListControl::updatePreset()
	{
		mPresets->setEnabled(getCurrentSkin() != nullptr);

		if (getCurrentSkin() != nullptr)
		{
			int currentPreset = 0;
			int bitIndex = 0;

			ItemHolder<SeparatorItem>::EnumeratorItem separators = getCurrentSkin()->getSeparators().getChildsEnumerator();
			while (separators.next())
			{
				SeparatorItem* item = separators.current();
				bool visible = item->getPropertySet()->getPropertyValue("Visible") == "True";
				if (visible)
					currentPreset |= (1 << bitIndex);

				++ bitIndex;
			}

			size_t indexSelected = MyGUI::ITEM_NONE;

			size_t count = mPresets->getItemCount();
			for (size_t index = 0; index < count; ++index)
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
