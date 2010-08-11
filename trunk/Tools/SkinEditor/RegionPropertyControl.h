/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __REGION_PROPERTY_CONTROL_H__
#define __REGION_PROPERTY_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "Property.h"
#include "SkinItem.h"
#include "PropertyAdvisor.h"
#include "PropertyInt4Control.h"
#include "PropertyBoolControl.h"
#include "PropertyAlignControl.h"
#include "PropertyRegionTypeControl.h"

namespace tools
{

	class RegionPropertyControl :
		public wraps::BaseLayout,
		public PropertyAdvisor
	{
	public:
		RegionPropertyControl(MyGUI::Widget* _parent);
		virtual ~RegionPropertyControl();

	private:
		virtual void updateRegionProperties();

	private:
		PropertyInt4Control* mPropertyPositionControl;
		PropertyBoolControl* mPropertyVisibleControl;
		PropertyBoolControl* mPropertyEnabledControl;
		PropertyAlignControl* mPropertyAlignControl;
		PropertyRegionTypeControl* mPropertyRegionTypeControl;
	};

} // namespace tools

#endif // __REGION_PROPERTY_CONTROL_H__
