#pragma once
#include "IFactory.h"

namespace components
{
	template <typename Type>
	class FactoryTemplate :
		public IFactory
	{
	public:
		FactoryTemplate() { }
		virtual ~FactoryTemplate() { }

		virtual IFactoryItem* CreateItem()
		{
			IFactoryItem* result = new Type();
			return result;
		}
	};
}