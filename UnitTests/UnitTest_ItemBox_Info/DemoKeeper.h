/*!
	@file
	@author		George Evmenov
	@date		08/2000
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

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

#endif // DEMO_KEEPER_H_
