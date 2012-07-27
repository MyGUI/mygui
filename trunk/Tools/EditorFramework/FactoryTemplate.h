/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _3d1be4a1_f606_48f1_9bda_b9c78c88a314_
#define _3d1be4a1_f606_48f1_9bda_b9c78c88a314_

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

#endif
