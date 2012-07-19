/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyUtility.h"
#include "FactoryManager.h"
#include "ActionManager.h"
#include "ActionChangeDataProperty.h"

namespace tools
{

	bool PropertyUtility::isUniqueName(Data* _data, const std::string& _propertyName)
	{
		Data* parent = _data->getParent();
		std::string name = _data->getPropertyValue(_propertyName);

		const Data::VectorData& childs = parent->getChilds();
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child++)
		{
			if ((*child)->getPropertyValue(_propertyName) == name && (*child) != _data)
				return false;
		}

		return true;
	}

	void PropertyUtility::executeAction(Property* _property, const std::string& _value, bool _merge)
	{
		if (_property->getValue() == _value)
			return;

		std::string actionName = _property->getType()->getAction();

		components::IFactoryItem* item = components::FactoryManager::GetInstance().CreateItem(actionName);
		if (item != nullptr)
		{
			ActionChangeDataProperty* action = dynamic_cast<ActionChangeDataProperty*>(item);
			if (action != nullptr)
			{
				action->setProperty(_property);
				action->setValue(_value);
				action->setMerge(_merge);

				ActionManager::getInstance().doAction(action);
				return;
			}
			else
			{
				delete item;
			}
		}
	}

	void PropertyUtility::storeUniqueNameProperty(const std::string& _propertyName, const std::string& _propertyUnique, Data* _parent, VectorPairProperty& _store)
	{
		const Data::VectorData& childs = _parent->getChilds();
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child++)
		{
			bool unique = isUniqueName((*child), _propertyName);
			Property* property = (*child)->getProperty(_propertyUnique);

			if (property->getValue<bool>() != unique)
			{
				_store.push_back(std::make_pair(property, property->getValue()));
				property->setValue(unique);
			}
		}
	}

	void PropertyUtility::restoreUniqueNameProperty(VectorPairProperty& _store)
	{
		for (VectorPairProperty::const_iterator value = _store.begin(); value != _store.end(); value ++)
			(*value).first->setValue((*value).second);
		_store.clear();
	}

}
