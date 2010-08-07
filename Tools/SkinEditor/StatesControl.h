/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATES_CONTROL_H__
#define __STATES_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "StatesListControl.h"

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
		//SkinPropertyControl* mSkinPropertyControl;
		//SkinTextureControl* mSkinTextureControl;
	};

} // namespace tools

#endif // __STATES_CONTROL_H__
