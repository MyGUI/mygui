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

	// wrapper class for removing data from a static vector
	template<typename Type>
	struct DataHolder
	{
		~DataHolder()
		{
			for (typename Type::iterator item = data.begin(); item != data.end(); ++item)
				delete (*item).first;
		}

		Type data;
	};

	// interface for field wrapper
	template<typename OwnerType, typename SetterType>
	struct Field
	{
		virtual ~Field() = default;
		virtual bool set(OwnerType* _target, typename SetterType::BaseValueType* _value) = 0;
		virtual std::string_view getFieldTypeName() const = 0;
	};

	// template for field wrapper
	template<typename OwnerType, typename FieldType, typename SetterType>
	struct FieldHolder : public Field<OwnerType, SetterType>
	{
		FieldHolder(FieldType* OwnerType::* offset) :
			m_offset(offset)
		{
		}
		FieldType* OwnerType::* const m_offset;

		bool set(OwnerType* _target, typename SetterType::BaseValueType* _value) override
		{
			_target->*m_offset = SetterType::template convert<FieldType>(_value);
			return _target->*m_offset != nullptr;
		}
		std::string_view getFieldTypeName() const override
		{
			return FieldType::getClassTypeName();
		}
	};

	// template for field attribute
	template<typename OwnerType, typename ValueType, typename SetterType>
	struct AttributeField
	{
		using BindPair = std::pair<Field<OwnerType, SetterType>*, ValueType>;
		using VectorBindPair = std::vector<BindPair>;

		template<typename FieldType>
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

	// macro for instantiating a field attribute
#define DECLARE_ATTRIBUTE_FIELD(_name, _type, _setter) \
	template<typename OwnerType, typename ValueType = _type, typename SetterType = _setter> \
	struct _name : public attribute::AttributeField<OwnerType, ValueType, SetterType> \
	{ \
		template<typename FieldType> \
		_name(FieldType* OwnerType::* _offset, const ValueType& _value) : \
			AttributeField<OwnerType, ValueType, SetterType>(_offset, _value) \
		{ \
		} \
	}

	// macro for instantiating an attribute instance
#define ATTRIBUTE_FIELD(_attribute, _class, _field, _value) \
	struct _attribute##_##_field \
	{ \
		_attribute##_##_field() \
		{ \
			static attribute::_attribute<_class> bind(&_class::_field, _value); \
		} \
	} _attribute##_##_field


	// template for class attribute
	template<typename Type, typename ValueType>
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

	// macro for instantiating a class attribute
#define DECLARE_ATTRIBUTE_CLASS(_name, _type) \
	template<typename Type, typename ValueType = _type> \
	struct _name : public attribute::ClassAttribute<_name<Type>, ValueType> \
	{ \
		_name(const ValueType& _value) : \
			ClassAttribute<_name<Type>, ValueType>(_value) \
		{ \
		} \
	}

	// macro for instantiating a class instance
#define ATTRIBUTE_CLASS(_attribute, _class, _value) \
	class _class; \
	static attribute::_attribute<_class> _attribute##_##_class(_value)
}

#endif // ATTRIBUTE_H_
