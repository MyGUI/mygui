/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __ACTION_MANAGER_H__
#define __ACTION_MANAGER_H__

#include <MyGUI.h>

namespace tools
{

	class ActionManager :
		public MyGUI::Singleton<ActionManager>
	{
	public:
		ActionManager();
		virtual ~ActionManager();

		void initialise();
		void shutdown();

		bool hasChanges();
	};

} // namespace tools

#endif // __ACTION_MANAGER_H__
