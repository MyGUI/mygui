/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "StateListControl.h"
#include "SkinManager.h"
#include "Binary.h"
#include "Localise.h"

namespace tools
{
	enum PresetEnum
	{
		PresetNormalOnly = Binary<10>::value,
		PresetFirst4States = Binary<1111>::value,
		PresetAllStates = Binary<11111111>::value
	};

	StatesListControl::StatesListControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("StateListControl.layout", _parent),
		mList(nullptr),
		mPresets(nullptr)
	{
		mTypeName = MyGUI::utility::toString((size_t)this);

		assignWidget(mList, "List");
		assignWidget(mPresets, "StatePreset");

		fillStatePreset();

		mList->eventListChangePosition += MyGUI::newDelegate(this, &StatesListControl::notifyChangePosition);
		mPresets->eventComboChangePosition += MyGUI::newDelegate(this, &StatesListControl::notifyComboChangePosition);

		initialiseAdvisor();
	}

	StatesListControl::~StatesListControl()
	{
		shutdownAdvisor();

		mList->eventListChangePosition -= MyGUI::newDelegate(this, &StatesListControl::notifyChangePosition);
		mPresets->eventComboChangePosition -= MyGUI::newDelegate(this, &StatesListControl::notifyComboChangePosition);
	}

	void StatesListControl::fillStatePreset()
	{
		mPresets->removeAllItems();
		mPresets->addItem(replaceTags("PresetStateOnlyNormal"), PresetNormalOnly);
		mPresets->addItem(replaceTags("PresetStateFirst4"), PresetFirst4States);
		mPresets->addItem(replaceTags("PresetStateAll"), PresetAllStates);

		mPresets->beginToItemFirst();
	}

	void StatesListControl::notifyChangePosition(MyGUI::ListBox* _sender, size_t _index)
	{
		if (getCurrentSkin() != nullptr)
		{
			StateItem* item = nullptr;

			if (_index != MyGUI::ITEM_NONE)
				item = *mList->getItemDataAt<StateItem*>(_index);

			getCurrentSkin()->getStates().setItemSelected(item);
		}
	}

	void StatesListControl::updateStateProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Visible")
			updateList();

		if (_owner != mTypeName)
			updatePreset();
	}

	void StatesListControl::updateStateProperties()
	{
		updateList();
	}

	void StatesListControl::updateSkinProperties()
	{
		updatePreset();
	}

	void StatesListControl::updateList()
	{
		size_t index = 0;

		if (getCurrentSkin() != nullptr)
		{
			StateItem* selectedItem = getCurrentSkin()->getStates().getItemSelected();
			size_t selectedIndex = MyGUI::ITEM_NONE;

			ItemHolder<StateItem>::EnumeratorItem states = getCurrentSkin()->getStates().getChildsEnumerator();
			while (states.next())
			{
				StateItem* item = states.current();

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

	void StatesListControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		if (getCurrentSkin() == nullptr)
			return;

		if (_index == MyGUI::ITEM_NONE)
			return;

		PresetEnum preset = *_sender->getItemDataAt<PresetEnum>(_index);

		size_t index = 0;
		ItemHolder<StateItem>::EnumeratorItem states = getCurrentSkin()->getStates().getChildsEnumerator();
		while (states.next())
		{
			StateItem* item = states.current();
			MyGUI::UString value = ((preset & (1 << index)) != 0) ? "True" : "False";
			item->getPropertySet()->setPropertyValue("Visible", value, mTypeName);

			++index;
		}

		updateList();
	}

	void StatesListControl::updatePreset()
	{
		mPresets->setEnabled(getCurrentSkin() != nullptr);

		if (getCurrentSkin() != nullptr)
		{
			int currentPreset = 0;
			int bitIndex = 0;

			ItemHolder<StateItem>::EnumeratorItem states = getCurrentSkin()->getStates().getChildsEnumerator();
			while (states.next())
			{
				StateItem* item = states.current();
				bool visible = item->getPropertySet()->getPropertyValue("Visible") == "True";
				if (visible)
					currentPreset |= (1 << bitIndex);

				++ bitIndex;
			}

			size_t indexSelected = MyGUI::ITEM_NONE;

			size_t count = mPresets->getItemCount();
			for (size_t index = 0; index < count; ++index)
			{
				PresetEnum preset = *mPresets->getItemDataAt<PresetEnum>(index);
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
