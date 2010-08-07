/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATES_CONTROL_H__
#define __STATES_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "StatesListControl.h"
#include "StatePropertyControl.h"
#include "StateTextureControl.h"

namespace tools
{

	class StatesControl :
		public wraps::BaseLayout
	{
	public:
		StatesControl(MyGUI::Widget* _parent);
		virtual ~StatesControl();

	private:
		StatesListControl* mStatesListControl;
		StatePropertyControl* mStatePropertyControl;
		StateTextureControl* mStateTextureControl;
	};

} // namespace tools

#endif // __STATES_CONTROL_H__
