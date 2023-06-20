/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

// -- Based on boost::any, original copyright information follows --
// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0.
// (See at http://www.boost.org/LICENSE_1_0.txt)
// -- End original copyright --

#ifndef MYGUI_ANY_H_
#define MYGUI_ANY_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Diagnostic.h"
#include <algorithm>

#include <typeinfo>

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

		template<typename ValueType>
		Any(const ValueType& value) :
			mContent(new Holder<ValueType>(value))
		{
		}

		~Any();

		Any& swap(Any& rhs);

		template<typename ValueType>
		Any& operator = (const ValueType& rhs)
		{
			Any(rhs).swap(*this);
			return *this;
		}

		Any& operator = (const Any& rhs);

		bool empty() const;

		const std::type_info& getType() const;

		template<typename ValueType>
		ValueType* castType(bool _throw = true) const
		{
			if (this->getType() == typeid(ValueType))
				return &static_cast<Any::Holder<ValueType> *>(this->mContent)->held;
			MYGUI_ASSERT(!_throw, "Bad cast from type '" << getType().name() << "' to '" << typeid(ValueType).name() << "'");
			return nullptr;
		}

		void* castUnsafe() const;

		bool compare(const Any& other) const;

	private:
		class Placeholder
		{
		public:
			virtual ~Placeholder() = default;

		public:
			virtual const std::type_info& getType() const = 0;
			virtual Placeholder* clone() const = 0;
			virtual bool compare(Placeholder* other) const = 0;
		};

		template<class T>
		struct HasOperatorEqualImpl
		{
			template <typename U>
			static auto test(U*) -> decltype(std::declval<U>() == std::declval<U>());
			template <typename>
			static auto test(...)->std::false_type;

			using type = typename std::is_same<bool, decltype(test<T>(nullptr))>::type;
			static constexpr bool value = type::value;
		};

		template<class T>
		struct HasOperatorEqual : HasOperatorEqualImpl<T>::type {};
		template<typename T1, typename T2>
		struct HasOperatorEqual<std::pair<T1, T2>>
		{
			static constexpr bool value = HasOperatorEqualImpl<T1>::value && HasOperatorEqualImpl<T2>::value;
		};

		template<typename ValueType>
		class Holder :
			public Placeholder
		{
			friend class Any;

		public:
			Holder(const ValueType& value) :
				held(value)
			{
			}

			Holder& operator=(const Holder&) = delete;

		public:
			const std::type_info& getType() const override
			{
				return typeid(ValueType);
			}

			Placeholder* clone() const override
			{
				return new Holder(held);
			}

			bool compare(Placeholder* other) const override
			{
				if constexpr (HasOperatorEqual<ValueType>::value)
					return getType() == other->getType() && held == static_cast<Holder*>(other)->held;
				else
					MYGUI_EXCEPT("Type '" << getType().name() << "' is not comparable");
			}

		private:
			ValueType held;
		};

	private:
		Placeholder* mContent{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_ANY_H_
