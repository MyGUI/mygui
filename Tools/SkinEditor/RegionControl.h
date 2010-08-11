/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __REGION_CONTROL_H__
#define __REGION_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "RegionListControl.h"
//#include "SkinPropertyControl.h"
//#include "SkinTextureControl.h"

namespace tools
{

	class RegionControl :
		public wraps::BaseLayout
	{
	public:
		RegionControl(MyGUI::Widget* _parent);
		virtual ~RegionControl();

	private:
		RegionListControl* mRegionListControl;
		//SkinPropertyControl* mSkinPropertyControl;
		//SkinTextureControl* mSkinTextureControl;
	};

} // namespace tools

#endif // __REGION_CONTROL_H__
