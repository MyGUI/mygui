/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataSelectorManager.h"

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

	void DataSelectorManager::changeParent(Data* _parent)
	{
		EventType* event = getEvent(_parent->getType()->getType());
		if (event != nullptr)
		{
			event->operator()(_parent, false);
		}
	}

	void DataSelectorManager::changeParentSelection(Data* _parent, Data* _selectedChild)
	{
		_parent->setChildSelected(_selectedChild);

		EventType* event = getEvent(_parent->getType()->getType());
		if (event != nullptr)
		{
			event->operator()(_parent, true);
		}
	}
}
