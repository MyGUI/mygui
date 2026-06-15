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

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;
		void setupResources() override;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
