/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyControl.h"
#include "FactoryManager.h"
#include "ActionChangeDataProperty.h"
#include "ActionManager.h"

namespace tools
{

	PropertyControl::PropertyControl() :
		mProperty(nullptr)
	{
	}

	PropertyControl::~PropertyControl()
	{
	}

	void PropertyControl::setProperty(Property* _value)
	{
		unadvice();
		mProperty = _value;
		advice();

		updateCaption();
		updateProperty();
	}

	Property* PropertyControl::getProperty()
	{
		return mProperty;
	}

	void PropertyControl::advice()
	{
		if (mProperty != nullptr)
		{
			mProperty->eventChangeProperty.connect(this, &PropertyControl::notifyChangeProperty);
		}
	}

	void PropertyControl::unadvice()
	{
		if (mProperty != nullptr)
		{
			mProperty->eventChangeProperty.disconnect(this);
			mProperty = nullptr;
		}
	}

	void PropertyControl::updateProperty()
	{
	}

	void PropertyControl::notifyChangeProperty(Property* _sender)
	{
		updateProperty();
	}

	void PropertyControl::updateCaption()
	{
	}

	void PropertyControl::executeAction(const std::string& _value)
	{
		std::string actionName = mProperty->getType()->getAction();

		components::IFactoryItem* item = components::FactoryManager::GetInstance().CreateItem(actionName);
		if (item != nullptr)
		{
			ActionChangeDataProperty* action = dynamic_cast<ActionChangeDataProperty*>(item);
			if (action != nullptr)
			{
				action->setProperty(mProperty);
				action->setValue(_value);

				ActionManager::getInstance().doAction(action);
				return;
			}
			else
			{
				delete item;
			}
		}
	}

}
