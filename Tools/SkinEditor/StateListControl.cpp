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

	StatesListControl::StatesListControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("StateListControl.layout", _parent),
		mList(nullptr)
	{
		assignWidget(mList, "List");

		mList->eventListChangePosition += MyGUI::newDelegate(this, &StatesListControl::notifyChangePosition);

		initialiseAdvisor();
	}

	StatesListControl::~StatesListControl()
	{
		shutdownAdvisor();

		mList->eventListChangePosition -= MyGUI::newDelegate(this, &StatesListControl::notifyChangePosition);
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

	void StatesListControl::updateStateProperty(Property* _sender, const MyGUI::UString& _value)
	{
		updateList();
	}

	void StatesListControl::updateStateProperties()
	{
		updateList();
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

				Property* prop = item->getPropertySet()->getChild("Visible");
				if (prop != nullptr && !isValueTrue(prop->getValue()))
					mList->addItem("#808080" + item->getName(), item);
				else
					mList->addItem(item->getName(), item);

				if (item == selectedItem)
					mList->setIndexSelected(index);
			}
		}
	}

	bool StatesListControl::isValueTrue(const MyGUI::UString& _value)
	{
		return (_value == "True");
	}

} // namespace tools
