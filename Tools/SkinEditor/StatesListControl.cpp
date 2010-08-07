/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "StatesListControl.h"
#include "SkinManager.h"

namespace tools
{

	StatesListControl::StatesListControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("StatesListControl.layout", _parent),
		mList(nullptr),
		mCurrentSkin(nullptr)
	{
		assignWidget(mList, "List");

		mList->eventListChangePosition += MyGUI::newDelegate(this, &StatesListControl::notifyChangePosition);

		SkinManager::getInstance().eventChangeSelection += MyGUI::newDelegate(this, &StatesListControl::notifyChangeSelection);
		advice();
	}

	StatesListControl::~StatesListControl()
	{
		SkinManager::getInstance().eventChangeSelection -= MyGUI::newDelegate(this, &StatesListControl::notifyChangeSelection);
		unadvice();

		mList->eventListChangePosition -= MyGUI::newDelegate(this, &StatesListControl::notifyChangePosition);
	}

	void StatesListControl::notifyChangePosition(MyGUI::List* _sender, size_t _index)
	{
		if (mCurrentSkin != nullptr)
		{
			StateItem* item = nullptr;

			if (_index != MyGUI::ITEM_NONE)
				item = *mList->getItemDataAt<StateItem*>(_index);

			mCurrentSkin->setItemSelected(item);
		}
	}

	void StatesListControl::notifyChangeSelection()
	{
		unadvice();
		advice();
	}

	void StatesListControl::unadvice()
	{
		if (mCurrentSkin != nullptr)
		{
			EnumeratorStateItem states = mCurrentSkin->getChildsEnumerator();
			while (states.next())
			{
				StateItem* item = states.current();
				item->getPropertySet()->eventChangeProperty -= MyGUI::newDelegate(this, &StatesListControl::notifyChangeProperty);
			}

			mCurrentSkin = nullptr;
			updateList();
		}
	}

	void StatesListControl::advice()
	{
		mCurrentSkin = SkinManager::getInstance().getItemSelected();

		if (mCurrentSkin != nullptr)
		{
			EnumeratorStateItem states = mCurrentSkin->getChildsEnumerator();
			while (states.next())
			{
				StateItem* item = states.current();
				item->getPropertySet()->eventChangeProperty += MyGUI::newDelegate(this, &StatesListControl::notifyChangeProperty);
			}

			updateList();
		}
	}

	void StatesListControl::notifyChangeProperty(Property* _sender, const MyGUI::UString& _value)
	{
		updateList();
	}

	void StatesListControl::updateList()
	{
		mList->removeAllItems();

		if (mCurrentSkin != nullptr)
		{
			EnumeratorStateItem states = mCurrentSkin->getChildsEnumerator();
			while (states.next())
			{
				StateItem* item = states.current();

				Property* prop = item->getPropertySet()->getChild("Visible");
				if (prop != nullptr && !isValueTrue(prop->getValue()))
					mList->addItem("#808080" + item->getName(), item);
				else
					mList->addItem(item->getName(), item);
			}
		}
	}

	bool StatesListControl::isValueTrue(const MyGUI::UString& _value)
	{
		bool result = true;
		if (!MyGUI::utility::parseComplex<bool>(_value, result))
			return false;
		return result;
	}

} // namespace tools
