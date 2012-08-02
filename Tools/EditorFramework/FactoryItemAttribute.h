/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _974be190_ed90_49c5_a20f_700745d10220_
#define _974be190_ed90_49c5_a20f_700745d10220_

#include "FactoryItemRegistrator.h"

#define FACTORY_ITEM_ATTRIBUTE(type) namespace { static ::factories::FactoryItemRegistrator<type> instance(#type); }

#define FACTORY_ITEM(type) ::factories::RegisterFactory(new ::components::FactoryTemplate<type>(), #type);

#endif
