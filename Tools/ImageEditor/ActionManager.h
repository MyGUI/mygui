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
	typedef MyGUI::delegates::CMultiDelegate1<bool> Event_Changes;

	class ActionManager :
		public MyGUI::Singleton<ActionManager>
	{
	public:
		ActionManager();
		virtual ~ActionManager();

		void initialise();
		void shutdown();

		bool getChanges() const;
		void setChanges(bool _value);

		Event_Changes eventChanges;

	private:
		bool mChanges;
	};

} // namespace tools

#endif // __ACTION_MANAGER_H__
