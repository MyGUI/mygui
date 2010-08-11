/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __PROPERTY_ALIGN_CONTROL_H__
#define __PROPERTY_ALIGN_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"

namespace tools
{

	class PropertyAlignControl :
		public wraps::BaseLayout,
		public PropertyControl
	{
	public:
		PropertyAlignControl(MyGUI::Widget* _parent);
		virtual ~PropertyAlignControl();

	protected:
		virtual void updateProperty();

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		size_t getComboIndex(const MyGUI::UString& _name);

	private:
		MyGUI::ComboBox* mComboBox;
	};

} // namespace tools

#endif // __PROPERTY_ALIGN_CONTROL_H__
