/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __PROPERTY_INT_4_CONTROL_H__
#define __PROPERTY_INT_4_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"

namespace tools
{

	class PropertyInt4Control :
		public wraps::BaseLayout,
		public PropertyControl
	{
	public:
		PropertyInt4Control(MyGUI::Widget* _parent);
		virtual ~PropertyInt4Control();

	protected:
		virtual void updateProperty();

	private:
		void notifyEditTextChange(MyGUI::Edit* _sender);

		bool isValidate();
		MyGUI::UString getClearValue();
		void setColour(bool _validate);

	private:
		MyGUI::Edit* mEdit;
	};

} // namespace tools

#endif // __PROPERTY_INT_4_CONTROL_H__
