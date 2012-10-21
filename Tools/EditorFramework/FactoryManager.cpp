#include "Precompiled.h"
#include "FactoryManager.h"

namespace components
{

	FactoryManager* FactoryManager::GetInstancePtr()
	{
		static FactoryManager instance;
		return &instance;
	}

	FactoryManager& FactoryManager::GetInstance()
	{
		return *GetInstancePtr();
	}

	FactoryManager::FactoryManager()
	{
	}

	FactoryManager::~FactoryManager()
	{
	}

	bool FactoryManager::ExistFactory(const std::string& _factoryName)
	{
		return mFactories.find(_factoryName) != mFactories.end();
	}

	void FactoryManager::RegisterFactory(IFactory* _factory, const std::string& _factoryName)
	{
		mFactories[_factoryName] = _factory;
	}

	IFactoryItem* FactoryManager::CreateItem(const std::string& _factoryName)
	{
		MapFactory::iterator item = mFactories.find(_factoryName);
		if (item != mFactories.end())
			return (*item).second->CreateItem();
		return 0;
	}

	void FactoryManager::UnregisterAllFactories()
	{
		for (MapFactory::iterator factory = mFactories.begin(); factory != mFactories.end(); factory ++)
			delete (*factory).second;
		mFactories.clear();
	}
}
