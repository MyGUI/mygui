/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATE_PROPERTY_CONTROL_H__
#define __STATE_PROPERTY_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "Property.h"
#include "SkinItem.h"
#include "PropertyAdvisor.h"
#include "PropertyBoolControl.h"
#include "PropertyInt2Control.h"
#include "PropertyIntControl.h"
#include "PropertyColourControl.h"

namespace tools
{

	class StatePropertyControl :
		public wraps::BaseLayout,
		public PropertyAdvisor
	{
	public:
		StatePropertyControl(MyGUI::Widget* _parent);
		virtual ~StatePropertyControl();

	private:
		virtual void updateStateProperties();

	private:
		PropertyBoolControl* mPropertyVisibleControl;
		PropertyInt2Control* mPropertyPositionControl;
		PropertyColourControl* mPropertyColourControl;
		PropertyIntControl* mPropertyOffsetControl;
	};

} // namespace tools

#endif // __STATE_PROPERTY_CONTROL_H__
