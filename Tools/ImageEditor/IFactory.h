/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _86dc80f6_1e36_4664_ace2_4c51c5ee10c4_
#define _86dc80f6_1e36_4664_ace2_4c51c5ee10c4_

#include "IFactoryItem.h"

namespace components
{

	class MYGUI_EXPORT_DLL IFactory
	{
	public:
		IFactory() { }
		virtual ~IFactory() { }

		virtual IFactoryItem* CreateItem() = 0;
	};

}

#endif
