/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

		virtual bool keyPressed( const OIS::KeyEvent &arg );

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
