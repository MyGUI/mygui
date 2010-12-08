/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __PROPERTY_INT_CONTROL_H__
#define __PROPERTY_INT_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"

namespace tools
{

	class PropertyIntControl :
		public wraps::BaseLayout,
		public PropertyControl
	{
	public:
		PropertyIntControl(MyGUI::Widget* _parent);
		virtual ~PropertyIntControl();

	protected:
		virtual void updateProperty();

	private:
		void notifyEditTextChange(MyGUI::EditBox* _sender);

		bool isValidate();
		MyGUI::UString getClearValue();
		void setColour(bool _validate);

	private:
		MyGUI::EditBox* mEdit;
	};

} // namespace tools

#endif // __PROPERTY_INT_CONTROL_H__
