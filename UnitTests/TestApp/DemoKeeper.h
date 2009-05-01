/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "IKinematicalObject.h"
#include "AbilityObject.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

		virtual bool mouseMoved( const OIS::MouseEvent &arg );
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
