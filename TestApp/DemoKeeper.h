/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

		virtual bool mouseMoved( const OIS::MouseEvent &arg );

		void createFont(int _res, int _height);

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
