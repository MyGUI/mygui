/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _a7965834_942c_4633_8ac1_01eedbd48d34_
#define _a7965834_942c_4633_8ac1_01eedbd48d34_

#include <MyGUI_Singleton.h>
#include "BaseLayout/BaseLayout.h"

namespace tools
{

	class ScopeManager :
		public MyGUI::Singleton<ScopeManager>
	{
	public:
		ScopeManager();
		virtual ~ScopeManager();

		void initialise();
		void shutdown();
	};

}

#endif
