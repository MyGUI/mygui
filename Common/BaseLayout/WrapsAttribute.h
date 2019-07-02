/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/

#ifndef WRAPS_ATTRIBUTE_H_
#define WRAPS_ATTRIBUTE_H_

#include <MyGUI.h>
#include "Attribute.h"

namespace attribute
{

	struct FieldSetterWidget
	{
		typedef MyGUI::Widget BaseValueType;

		template <typename Type>
		static Type* convert(BaseValueType* _value)
		{
			return _value == nullptr ? nullptr : _value->castType<Type>(false);
		}
	};

	DECLARE_ATTRIBUTE_FIELD(AttributeFieldWidgetName, std::string, FieldSetterWidget);

#define ATTRIBUTE_FIELD_WIDGET_NAME(_class, _field, _value) \
	ATTRIBUTE_FIELD(AttributeFieldWidgetName, _class, _field, _value)


	DECLARE_ATTRIBUTE_CLASS(AttributeLayout, std::string);

#define ATTRIBUTE_CLASS_LAYOUT(_class, _value) \
	ATTRIBUTE_CLASS(AttributeLayout, _class, _value)

}

#endif // WRAPS_ATTRIBUTE_H_
