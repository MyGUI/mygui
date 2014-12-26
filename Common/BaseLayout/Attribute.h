/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/

#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

namespace attribute
{

	// класс обертка для удаления данных из статического вектора
	template <typename Type>
	struct DataHolder
	{
		~DataHolder()
		{
			for (typename Type::iterator item = data.begin(); item != data.end(); ++item)
				delete (*item).first;
		}

		Type data;
	};

	// интерфейс для обертки поля
	template <typename OwnerType, typename SetterType>
	struct Field
	{
		virtual ~Field() { }
		virtual bool set(OwnerType* _target, typename SetterType::BaseValueType* _value) = 0;
		virtual const std::string& getFieldTypeName() = 0;
	};

	// шаблон для обертки поля
	template <typename OwnerType, typename FieldType, typename SetterType>
	struct FieldHolder : public Field<OwnerType, SetterType>
	{
		FieldHolder(FieldType* OwnerType::* offset) : m_offset(offset) {  }
		FieldType* OwnerType::* const m_offset;

		virtual bool set(OwnerType* _target, typename SetterType::BaseValueType* _value)
		{
			_target->*m_offset = SetterType::template convert<FieldType>(_value);
			return _target->*m_offset != 0;
		}
		virtual const std::string& getFieldTypeName()
		{
			return FieldType::getClassTypeName();
		}
	};

	// шаблон для атрибута поля
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
			static DataHolder<VectorBindPair> data;
			return data.data;
		}
	};

	// макрос для инстансирования атрибута поля
#define DECLARE_ATTRIBUTE_FIELD(_name, _type, _setter) \
	template <typename OwnerType, typename ValueType = _type, typename SetterType = _setter> \
	struct _name : public attribute::AttributeField<OwnerType, ValueType, SetterType> \
	{ \
		template <typename FieldType> \
		_name(FieldType* OwnerType::* _offset, const ValueType& _value) : \
			AttributeField<OwnerType, ValueType, SetterType>(_offset, _value) { } \
	}

	// макрос для инстансирования экземпляра атрибута
#define ATTRIBUTE_FIELD(_attribute, _class, _field, _value) \
	struct _attribute##_##_field \
	{ \
		_attribute##_##_field() \
		{ \
			static attribute::_attribute<_class> bind(&_class::_field, _value); \
		} \
	} _attribute##_##_field


	// шаблон для атрибута класса
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

	// макрос для инстансирования атрибута класса
#define DECLARE_ATTRIBUTE_CLASS(_name, _type) \
	template <typename Type, typename ValueType = _type> \
	struct _name : public attribute::ClassAttribute<_name<Type>, ValueType> \
	{ \
		_name(const ValueType& _value) : \
			ClassAttribute<_name<Type>, ValueType>(_value) { } \
	}

	// макрос для инстансирования экземпляра класса
#define ATTRIBUTE_CLASS(_attribute, _class, _value) \
	class _class; \
	static attribute::_attribute<_class> _attribute##_##_class(_value)
}

#endif // ATTRIBUTE_H_
