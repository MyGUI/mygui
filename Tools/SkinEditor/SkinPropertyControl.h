/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_PROPERTY_CONTROL_H__
#define __SKIN_PROPERTY_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "Property.h"
#include "SkinItem.h"
#include "PropertyAdvisor.h"
#include "PropertyInt4Control.h"
#include "PropertyTexturesControl.h"

namespace tools
{

	class SkinPropertyControl :
		public wraps::BaseLayout,
		public PropertyAdvisor
	{
	public:
		SkinPropertyControl(MyGUI::Widget* _parent);
		virtual ~SkinPropertyControl();

	private:
		virtual void updateSkinProperties();

	private:
		PropertyInt4Control* mPropertyCoordControl;
		PropertyTexturesControl* mPropertyTexturesControl;
	};

} // namespace tools

#endif // __SKIN_PROPERTY_CONTROL_H__
