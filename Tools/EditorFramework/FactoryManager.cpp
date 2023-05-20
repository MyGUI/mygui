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

	bool FactoryManager::ExistFactory(std::string_view _factoryName)
	{
		return mFactories.find(_factoryName) != mFactories.end();
	}

	void FactoryManager::RegisterFactory(IFactory* _factory, std::string_view _factoryName)
	{
		MyGUI::mapSet(mFactories, _factoryName, _factory);
	}

	IFactoryItem* FactoryManager::CreateItem(std::string_view _factoryName)
	{
		MapFactory::iterator item = mFactories.find(_factoryName);
		if (item != mFactories.end())
			return (*item).second->CreateItem();
		return nullptr;
	}

	void FactoryManager::UnregisterAllFactories()
	{
		for (auto& factory : mFactories)
			delete factory.second;
		mFactories.clear();
	}
}
