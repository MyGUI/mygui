/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseDemoManager.h"
#include "State.h"

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
		void notifyButtonPress(ControllerType _type, bool _complite);
		virtual void setupResources();

	private:
		State* mMenu1;
		State* mMenu2;
		State* mMenu3;
		State* mMenu4;
		ControllerType mType;
		bool mIsAnim;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
