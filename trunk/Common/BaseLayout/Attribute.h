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
#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

#include <MyGUI.h>

namespace attribute
{

	template <typename OwnerType, typename SetterType>
	struct Field
	{
		virtual void set(OwnerType* _target, typename SetterType::BaseValueType* _value) = 0;
	};

	template <typename OwnerType, typename FieldType, typename SetterType>
	struct FieldHolder : public Field<OwnerType, SetterType>
	{
		FieldHolder(FieldType* OwnerType::* offset) : m_offset(offset) {  }
		FieldType* OwnerType::* const m_offset;

		virtual void set(OwnerType* _target, typename SetterType::BaseValueType* _value)
		{
			_target->*m_offset = SetterType::convert<FieldType>(_value);
		}
	};

	template <typename OwnerType, typename ValueType, typename SetterType>
	struct AttributeField
	{
		typedef std::pair<Field<OwnerType, SetterType>*, ValueType> BindPair;
		typedef std::vector<BindPair> VectorBindPair;

		template <typename FieldType>
		AttributeField(FieldType* OwnerType::* _offset, const ValueType& _value)
		{
			getData().push_back(BindPair(new FieldHolder<OwnerType, FieldType, SetterType>(_offset), _value));
		}
		static VectorBindPair& getData()
		{
			static VectorBindPair data;
			return data;
		}
	};

#define DECLARE_ATTRIBUTE_FIELD(_name, _type, _setter) \
	template <typename OwnerType, typename ValueType = _type, typename SetterType = _setter> \
	struct _name : public attribute::AttributeField<OwnerType, ValueType, SetterType> \
	{ \
		template <typename FieldType> \
		_name(FieldType* OwnerType::* _offset, const ValueType& _value) : \
			AttributeField<OwnerType, ValueType, SetterType>(_offset, _value) { } \
	};

#define ATTRIBUTE_FIELD(_attribute, _class, _field, _value) \
	struct _attribute##_##_field \
	{ \
		_attribute##_##_field() \
		{ \
			static wraps::_attribute<_class> bind(&_class::_field, _value); \
		} \
	} _attribute##_##_field;



	template <typename Type, typename ValueType>
	struct ClassAttribute
	{
		ClassAttribute(const ValueType& _value)
		{
			getData() = _value;
		}
		static ValueType& getData()
		{
			static ValueType data;
			return data;
		}
	};

#define DECLARE_ATTRIBUTE_CLASS(_name, _type) \
	template <typename Type, typename ValueType = _type> \
	struct _name : public attribute::ClassAttribute<_name<Type>, ValueType> \
	{ \
		_name(const ValueType& _value) : \
			ClassAttribute<_name<Type>, ValueType>(_value) { } \
	};

#define ATTRIBUTE_CLASS(_attribute, _class, _value) \
	class _class; \
	static wraps::_attribute<_class> _attribute##_##_class(_value);
}

#endif // __ATTRIBUTE_H__
