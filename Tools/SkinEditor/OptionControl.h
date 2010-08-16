/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __OPTION_CONTROL_H__
#define __OPTION_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "OptionPathControl.h"
//#include "RegionPropertyControl.h"
//#include "RegionTextureControl.h"

namespace tools
{

	class OptionControl :
		public wraps::BaseLayout
	{
	public:
		OptionControl(MyGUI::Widget* _parent);
		virtual ~OptionControl();

	private:
		OptionPathControl* mOptionPathControl;
		//RegionPropertyControl* mRegionPropertyControl;
		//RegionTextureControl* mRegionTextureControl;
	};

} // namespace tools

#endif // __OPTION_CONTROL_H__
