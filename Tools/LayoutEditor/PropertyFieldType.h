/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_TYPE_H__
#define __PROPERTY_FIELD_TYPE_H__

#include "EditorToolTip.h"
#include "PropertyFieldComboBox.h"

namespace tools
{
	class PropertyFieldType :
		public PropertyFieldComboBox
	{
	public:
		PropertyFieldType();
		virtual ~PropertyFieldType();

	protected:
		virtual void onFillValues();
		virtual void onAction(const std::string& _value);
	};

} // namespace tools

#endif // __PROPERTY_FIELD_TYPE_H__
