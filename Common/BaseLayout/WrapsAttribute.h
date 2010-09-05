/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/

#ifndef __WRAPS_ATTRIBUTE_H__
#define __WRAPS_ATTRIBUTE_H__

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
			return _value == 0 ? 0 : _value->castType<Type>(false);
		}
	};

	DECLARE_ATTRIBUTE_FIELD(AttributeFieldWidgetName, std::string, FieldSetterWidget);

#define ATTRIBUTE_FIELD_WIDGET_NAME(_class, _field, _value) \
	ATTRIBUTE_FIELD(AttributeFieldWidgetName, _class, _field, _value)


	DECLARE_ATTRIBUTE_CLASS(AttributeSize, MyGUI::IntSize);

#define ATTRIBUTE_CLASS_SIZE(_class, _value) \
	ATTRIBUTE_CLASS(AttributeSize, _class, _value)


	DECLARE_ATTRIBUTE_CLASS(AttributeLayout, std::string);

#define ATTRIBUTE_CLASS_LAYOUT(_class, _value) \
	ATTRIBUTE_CLASS(AttributeLayout, _class, _value)

}

#endif // __WRAPS_ATTRIBUTE_H__
