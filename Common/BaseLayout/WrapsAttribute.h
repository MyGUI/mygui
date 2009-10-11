/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __WRAPS_ATTRIBUTE_H__
#define __WRAPS_ATTRIBUTE_H__

#include "Attribute.h"

namespace wraps
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
