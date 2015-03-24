/*!
	@file
	@author		George Evmenov
	@date		02/2011
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"

namespace demo
{

	class BerkeliumBrowser;

	class DemoKeeper :
		public base::BaseManager
	{
	public:
		DemoKeeper();
		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual void setupResources();

	private:
		BerkeliumBrowser* mBerkeliumBrowser;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
