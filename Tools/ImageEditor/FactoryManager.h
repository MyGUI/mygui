#pragma once
#include <string>
#include <map>
#include "IFactory.h"
#include "FactoryTemplate.h"
#include "FactoryItemAttribute.h"

namespace components
{
	class FactoryManager
	{
	public:
		FactoryManager();
		~FactoryManager();

		static FactoryManager* GetInstancePtr();
		static FactoryManager& GetInstance();

		bool ExistFactory(const std::string& _factoryName);

		void RegisterFactory(IFactory* _factory, const std::string& _factoryName);
		void UnregisterFactory(IFactory* _factory, const std::string& _factoryName);

		IFactoryItem* CreateItem(const std::string& _factoryName);

	private:
		typedef std::map<std::string, IFactory*> MapFactory;
		MapFactory mFactories;
	};
}
