/*!
	@file
	@author		Albert Semenov
	@date		12/2009
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
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

	private:
		void setupResources();
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
