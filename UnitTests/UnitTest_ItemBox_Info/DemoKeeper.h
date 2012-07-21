/*!
	@file
	@author		George Evmenov
	@date		08/2000
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseDemoManager.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseDemoManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();
		virtual void setupResources();
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
