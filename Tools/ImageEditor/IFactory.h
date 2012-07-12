#pragma once
#include "IFactoryItem.h"

namespace components
{
	class IFactory
	{
	public:
		IFactory() { }
		virtual ~IFactory() { }

		virtual IFactoryItem* CreateItem() = 0;
	};
}
