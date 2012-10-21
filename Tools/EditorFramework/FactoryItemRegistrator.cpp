/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "FactoryItemRegistrator.h"
#include "FactoryManager.h"

namespace factories
{
	bool IsExistFactoryName(const std::string& _factoryName)
	{
		return ::components::FactoryManager::GetInstancePtr()->ExistFactory(_factoryName);
	}

	void RegisterFactory(::components::IFactory* _factory, const std::string& _factoryName)
	{
		::components::FactoryManager::GetInstancePtr()->RegisterFactory(_factory, _factoryName);
	}
}
