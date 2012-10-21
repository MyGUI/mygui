/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _60e2a077_d22d_435e_866e_ac48372e1eed_
#define _60e2a077_d22d_435e_866e_ac48372e1eed_

#include <string>
#include <map>
#include "IFactory.h"
#include "FactoryTemplate.h"
#include "FactoryItemRegistrator.h"
#include "FactoryItemAttribute.h"

namespace components
{

	class MYGUI_EXPORT_DLL FactoryManager
	{
	public:
		FactoryManager();
		~FactoryManager();

		static FactoryManager* GetInstancePtr();
		static FactoryManager& GetInstance();

		bool ExistFactory(const std::string& _factoryName);

		void RegisterFactory(IFactory* _factory, const std::string& _factoryName);
		void UnregisterAllFactories();

		IFactoryItem* CreateItem(const std::string& _factoryName);

		template <typename Type>
		Type* CreateItem(const std::string& _factoryName)
		{
			IFactoryItem* item = CreateItem(_factoryName);
			if (item != nullptr)
			{
				Type* result = dynamic_cast<Type*>(item);
				if (result != nullptr)
					return result;
				delete item;
			}
			return nullptr;
		}

	private:
		typedef std::map<std::string, IFactory*> MapFactory;
		MapFactory mFactories;
	};

}

#endif
