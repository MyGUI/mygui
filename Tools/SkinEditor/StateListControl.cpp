/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "StateListControl.h"
#include "SkinManager.h"

namespace tools
{
	enum PresetEnum
	{
		PresetCustom,
		PresetNormalOnly,
		PresetFirst4States,
		PresetAllStates
	};

	StatesListControl::StatesListControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("StateListControl.layout", _parent),
		mList(nullptr),
		mStatePreset(nullptr)
	{
		mTypeName = MyGUI::utility::toString((int)this);

		assignWidget(mList, "List");
		assignWidget(mStatePreset, "StatePreset");

		fillStatePreset();

		mList->eventListChangePosition += MyGUI::newDelegate(this, &StatesListControl::notifyChangePosition);
		mStatePreset->eventComboChangePosition += MyGUI::newDelegate(this, &StatesListControl::notifyComboChangePosition);

		initialiseAdvisor();
	}

	StatesListControl::~StatesListControl()
	{
		shutdownAdvisor();

		mList->eventListChangePosition -= MyGUI::newDelegate(this, &StatesListControl::notifyChangePosition);
		mStatePreset->eventComboChangePosition -= MyGUI::newDelegate(this, &StatesListControl::notifyComboChangePosition);
	}

	void StatesListControl::fillStatePreset()
	{
		mStatePreset->addItem("Custom", PresetCustom);
		mStatePreset->addItem("Normal only", PresetNormalOnly);
		mStatePreset->addItem("First 4 states", PresetFirst4States);
		mStatePreset->addItem("All states", PresetAllStates);
	}

	void StatesListControl::notifyChangePosition(MyGUI::List* _sender, size_t _index)
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
		mList->setIndexSelected(MyGUI::ITEM_NONE);
		mList->removeAllItems();

		if (getCurrentSkin() != nullptr)
		{
			StateItem* selectedItem = getCurrentSkin()->getStates().getItemSelected();

			ItemHolder<StateItem>::EnumeratorItem states = getCurrentSkin()->getStates().getChildsEnumerator();
			while (states.next())
			{
				size_t index = mList->getItemCount();

				StateItem* item = states.current();

				if (item->getPropertySet()->getPropertyValue("Visible") != "True")
					mList->addItem("#808080" + item->getName(), item);
				else
					mList->addItem(item->getName(), item);

				if (item == selectedItem)
					mList->setIndexSelected(index);
			}
		}
	}

	void StatesListControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		if (getCurrentSkin() == nullptr)
			return;

		if (_index == MyGUI::ITEM_NONE)
			return;

		PresetEnum preset = *_sender->getItemDataAt<PresetEnum>(_index);

		if (preset == PresetCustom)
			return;

		//selectPreset(preset);

		if (preset == PresetNormalOnly)
		{
			StateItem* selectedItem = getCurrentSkin()->getStates().getItemSelected();

			size_t index = 0;
			ItemHolder<StateItem>::EnumeratorItem states = getCurrentSkin()->getStates().getChildsEnumerator();
			while (states.next())
			{
				StateItem* item = states.current();
				MyGUI::UString value = index == 1 ? "True" : "False";
				item->getPropertySet()->setPropertyValue("Visible", value, mTypeName);

				++index;
			}
		}
		else if (preset == PresetFirst4States)
		{
			StateItem* selectedItem = getCurrentSkin()->getStates().getItemSelected();

			size_t index = 0;
			ItemHolder<StateItem>::EnumeratorItem states = getCurrentSkin()->getStates().getChildsEnumerator();
			while (states.next())
			{
				StateItem* item = states.current();
				MyGUI::UString value = index < 4 ? "True" : "False";
				item->getPropertySet()->setPropertyValue("Visible", value, mTypeName);

				++index;
			}
		}
		else if (preset == PresetAllStates)
		{
			StateItem* selectedItem = getCurrentSkin()->getStates().getItemSelected();

			size_t index = 0;
			ItemHolder<StateItem>::EnumeratorItem states = getCurrentSkin()->getStates().getChildsEnumerator();
			while (states.next())
			{
				StateItem* item = states.current();
				MyGUI::UString value = "True";
				item->getPropertySet()->setPropertyValue("Visible", value, mTypeName);

				++index;
			}
		}

		updateList();
	}

	void StatesListControl::updatePreset()
	{
		mStatePreset->setEnabled(getCurrentSkin() != nullptr);

		if (getCurrentSkin() != nullptr)
		{
			PresetEnum currentPresent = PresetCustom;
			MyGUI::UString result;

			ItemHolder<StateItem>::EnumeratorItem states = getCurrentSkin()->getStates().getChildsEnumerator();
			while (states.next())
			{
				StateItem* item = states.current();
				bool visible = item->getPropertySet()->getPropertyValue("Visible") == "True";
				result.append(visible ? "1" : "0");
			}

			if (result == "01000000")
				currentPresent = PresetNormalOnly;
			else if (result == "11110000")
				currentPresent = PresetFirst4States;
			else if (result == "11111111")
				currentPresent = PresetAllStates;

			size_t indexSelected = MyGUI::ITEM_NONE;

			size_t count = mStatePreset->getItemCount();
			for (size_t index=0; index<count; ++index)
			{
				PresetEnum preset = *mStatePreset->getItemDataAt<PresetEnum>(index);
				if (preset == currentPresent)
				{
					indexSelected = index;
					break;
				}
			}

			mStatePreset->setIndexSelected(indexSelected);

			mStatePreset->setEnabled(true);
		}
		else
		{
			mStatePreset->setEnabled(false);
		}

	}

} // namespace tools
