/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_ANY_H_
#define MYGUI_ANY_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Diagnostic.h"
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace MyGUI
{

	/** @example "Class Any usage"
	@code
	void f()
	{
		// test class, with simple types everything is similar
		struct Data { int value; };

		// instance and initialization
		Data data;
		data.value = 0xDEAD;

		// copy of class Data will be created
		MyGUI::Any any = data;
		// copy of class Data
		Data copy_data = *any.castType<Data>();
		// now value == 0xDEAD
		int value = copy_data.value;


		// copy of pointer on class Data will be created
		any = &data;
		// copy of pointer on class Data and on object data
		Data* copy_ptr = *any.castType<Data*>();
		// now value == 0
		copy_ptr->value = 0;
	}
	@endcode
	*/

	class MYGUI_EXPORT Any
	{
	public:
		static const Any Null;
		Any();
		Any(const Any& other);
		Any(Any&& other) noexcept;
		~Any();

		template<typename ValueType>
		Any(const ValueType& value) :
			mData(::new std::decay_t<const ValueType>(value)),
			mType(getTypeOps<std::decay_t<const ValueType>>())
		{
		}

		template<typename ValueType>
		Any& operator=(const ValueType& rhs)
		{
			return *this = Any(rhs);
		}

		Any& operator=(const Any& rhs);
		Any& operator=(Any&& rhs) noexcept;

		bool empty() const;

		const std::type_info& getType() const;

		template<typename ValueType>
		ValueType* castType(bool _throw = true)
		{
			if constexpr (std::is_object_v<ValueType>)
			{
				if (mData && mType->type == typeid(ValueType))
					return static_cast<ValueType*>(mData);
			}
			if (_throw)
				detail::throwBadCast(getType().name(), typeid(ValueType).name(), __FILE__, __LINE__);
			return nullptr;
		}

		template<typename ValueType>
		const ValueType* castType(bool _throw = true) const
		{
			if constexpr (std::is_object_v<ValueType>)
			{
				if (mData && mType->type == typeid(ValueType))
					return static_cast<const ValueType*>(mData);
			}
			if (_throw)
				detail::throwBadCast(getType().name(), typeid(ValueType).name(), __FILE__, __LINE__);
			return nullptr;
		}

		bool compare(const Any& other) const;

	private:
		template<typename T, typename = void>
		struct HasOperatorEqual : std::false_type
		{
		};

		template<typename T>
		struct HasOperatorEqual<T, std::void_t<decltype(std::declval<const T&>() == std::declval<const T&>())>> :
			std::is_same<bool, decltype(std::declval<const T&>() == std::declval<const T&>())>
		{
		};

		template<typename T1, typename T2>
		struct HasOperatorEqual<std::pair<T1, T2>> : std::conjunction<HasOperatorEqual<T1>, HasOperatorEqual<T2>>
		{
		};

		struct TypeOps
		{
			const std::type_info& type;
			void* (*copy)(const void*);
			void (*destroy)(void*);
			bool (*compare)(const void*, const void*, const std::type_info&);
		};

		template<typename T>
		static const TypeOps* getTypeOps() noexcept
		{
			static constexpr TypeOps operations{
				typeid(T),
				[](const void* source) -> void* { return ::new T(*static_cast<const T*>(source)); },
				[](void* source) { ::delete static_cast<T*>(source); },
				[](const void* left, const void* right, const std::type_info& rightType) -> bool
				{
					if constexpr (HasOperatorEqual<T>::value)
						return typeid(T) == rightType && *static_cast<const T*>(left) == *static_cast<const T*>(right);
					else
						MYGUI_EXCEPT("Type '" << typeid(T).name() << "' is not comparable");
				}};
			return &operations;
		}

	private:
		// The allocation contains only the value; its operations are shared by type.
		// Moving an Any transfers ownership without relocating its stored value.
		void* mData = nullptr;
		const TypeOps* mType = nullptr;
	};

} // namespace MyGUI

#endif // MYGUI_ANY_H_
