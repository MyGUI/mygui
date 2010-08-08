/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SEPARATOR_PROPERTY_CONTROL_H__
#define __SEPARATOR_PROPERTY_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "Property.h"
#include "SkinItem.h"
#include "PropertyAdvisor.h"
#include "PropertyBoolControl.h"
#include "PropertyIntControl.h"

namespace tools
{

	class SeparatorPropertyControl :
		public wraps::BaseLayout,
		public PropertyAdvisor
	{
	public:
		SeparatorPropertyControl(MyGUI::Widget* _parent);
		virtual ~SeparatorPropertyControl();

	private:
		virtual void updateSeparatorProperties();

	private:
		PropertyBoolControl* mPropertyVisibleControl;
		PropertyIntControl* mPropertyPositionControl;
	};

} // namespace tools

#endif // __SEPARATOR_PROPERTY_CONTROL_H__
