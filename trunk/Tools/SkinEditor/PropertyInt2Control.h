/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __PROPERTY_INT_2_CONTROL_H__
#define __PROPERTY_INT_2_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"

namespace tools
{

	class PropertyInt2Control :
		public wraps::BaseLayout,
		public PropertyControl
	{
	public:
		PropertyInt2Control(MyGUI::Widget* _parent);
		virtual ~PropertyInt2Control();

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

#endif // __PROPERTY_INT_2_CONTROL_H__
