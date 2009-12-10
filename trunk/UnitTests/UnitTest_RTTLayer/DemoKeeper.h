/*!
	@file
	@author		Albert Semenov
	@date		12/2009
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseManager
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

	private:
		void setupResources();
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
