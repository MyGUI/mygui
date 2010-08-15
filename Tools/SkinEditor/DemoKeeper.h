/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "MainPane.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseManager,
		public MyGUI::Singleton<DemoKeeper>
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual void setupResources();

	private:
		tools::MainPane* mMainPane;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
