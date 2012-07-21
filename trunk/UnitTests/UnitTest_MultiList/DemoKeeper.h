/*!
	@file
	@author		Albert Semenov
	@date		10/2008
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

	private:
		virtual void setupResources();
		void notifyFrameStarted(float _time);

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
