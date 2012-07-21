/*!
	@file
	@author		George Evmenov
	@date		01/2011
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

		void handleClick_MemberFunction(MyGUI::Widget* _sender);
		static void handleClick_StaticMemberFunction(MyGUI::Widget* _sender);
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
