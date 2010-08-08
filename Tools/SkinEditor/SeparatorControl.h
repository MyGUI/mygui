/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SEPARATOR_CONTROL_H__
#define __SEPARATOR_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "SeparatorListControl.h"
#include "SeparatorPropertyControl.h"
//#include "StateTextureControl.h"

namespace tools
{

	class SeparatorControl :
		public wraps::BaseLayout
	{
	public:
		SeparatorControl(MyGUI::Widget* _parent);
		virtual ~SeparatorControl();

	private:
		SeparatorListControl* mSeparatorListControl;
		SeparatorPropertyControl* mSeparatorPropertyControl;
		//StateTextureControl* mStateTextureControl;
	};

} // namespace tools

#endif // __SEPARATOR_CONTROL_H__
