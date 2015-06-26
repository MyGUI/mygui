/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataSelectorManager.h"
#include "DataTypeManager.h"

namespace tools
{

	DataSelectorManager* DataSelectorManager::mInstance = nullptr;

	DataSelectorManager::DataSelectorManager()
	{
		mInstance = this;
	}

	DataSelectorManager::~DataSelectorManager()
	{
		mInstance = nullptr;
	}

	DataSelectorManager& DataSelectorManager::getInstance()
	{
		return *mInstance;
	}

	DataSelectorManager* DataSelectorManager::getInstancePtr()
	{
		return mInstance;
	}

	void DataSelectorManager::initialise()
	{
	}

	void DataSelectorManager::shutdown()
	{
		clear();
	}

	void DataSelectorManager::clear()
	{
		for (MapEvent::iterator event = mEvents.begin(); event != mEvents.end(); event ++)
			delete (*event).second;
		mEvents.clear();
	}

	DataSelectorManager::EventType* DataSelectorManager::getEvent(const std::string& _dataType)
	{
		MapEvent::iterator event = mEvents.find(_dataType);
		if (event != mEvents.end())
			return (*event).second;

		EventType* type = new EventType();
		mEvents[_dataType] = type;
		return type;
	}

	void DataSelectorManager::changeParent(DataPtr _parent)
	{
		onChangeData(_parent, _parent->getType(), false);
	}

	void DataSelectorManager::changeParentSelection(DataPtr _parent, DataPtr _selectedChild)
	{
		_parent->setChildSelected(_selectedChild);

		onChangeData(_parent, _parent->getType(), true);
	}

	void DataSelectorManager::onChangeData(DataPtr _parent, DataTypePtr _type, bool _changeOnlySelection)
	{
		EventType* event = getEvent(_type->getName());
		if (event != nullptr)
		{
			event->operator()(_parent, _changeOnlySelection);
		}

		DataPtr childSelected = nullptr;
		if (_parent != nullptr)
			childSelected = _parent->getChildSelected();

		const DataType::VectorString& childs = _type->getChilds();
		for (DataType::VectorString::const_iterator childName = childs.begin(); childName != childs.end(); childName ++)
		{
			DataTypePtr childType = DataTypeManager::getInstance().getType(*childName);
			if (childType != nullptr)
			{
				DataPtr child = childSelected;
				if (child != nullptr && child->getType() != childType)
					child = nullptr;

				if (child != nullptr)
				{
					DataPtr subChildSelected = child->getChildSelected();
					if (subChildSelected == nullptr)
					{
						if (child->getChilds().size() != 0)
						{
							DataPtr childData = child->getChildByIndex(0);
							child->setChildSelected(childData);
						}
					}
				}

				onChangeData(child, childType, false);
			}
		}
	}

}
