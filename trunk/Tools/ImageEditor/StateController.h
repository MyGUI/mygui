/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATE_CONTROLLER_H__
#define __STATE_CONTROLLER_H__

#include <MyGUI.h>

namespace tools
{
	class StateController
	{
	public:
		StateController() { }
		virtual ~StateController() { }

		virtual void initState() { }
		virtual void cleanupState() { }

		virtual void pauseState() { }
		virtual void resumeState() { }
	};

} // namespace tools

#endif // __STATE_CONTROLLER_H__
