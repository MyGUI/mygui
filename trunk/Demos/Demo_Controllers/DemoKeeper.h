/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "Menu1.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

	private:
		void notifyButtonPress(size_t _index);

	private:
		Menu1 mMenu1;
		Menu1 mMenu2;
		Menu1 mMenu3;
		Menu1 mMenu4;
		size_t mIndex;

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
