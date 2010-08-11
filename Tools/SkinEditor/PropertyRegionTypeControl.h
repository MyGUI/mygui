/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __PROPERTY_REGION_TYPE_CONTROL_H__
#define __PROPERTY_REGION_TYPE_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"

namespace tools
{

	class PropertyRegionTypeControl :
		public wraps::BaseLayout,
		public PropertyControl
	{
	public:
		PropertyRegionTypeControl(MyGUI::Widget* _parent);
		virtual ~PropertyRegionTypeControl();

	protected:
		virtual void updateProperty();

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		size_t getComboIndex(const MyGUI::UString& _name);

	private:
		MyGUI::ComboBox* mComboBox;
	};

} // namespace tools

#endif // __PROPERTY_REGION_TYPE_CONTROL_H__
