/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

		void notifyFrameStarted(float _time);
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
