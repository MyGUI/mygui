/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_ALIGN_H__
#define __PROPERTY_FIELD_ALIGN_H__

#include "EditorToolTip.h"
#include "PropertyFieldComboBox.h"

namespace tools
{
	class PropertyFieldAlign :
		public PropertyFieldComboBox
	{
	public:
		PropertyFieldAlign(MyGUI::Widget* _parent);
		virtual ~PropertyFieldAlign();

	protected:
		virtual void onAction(const std::string& _value);
	};

} // namespace tools

#endif // __PROPERTY_FIELD_ALIGN_H__
