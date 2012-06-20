/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "Utility.h"

namespace MyGUI
{
	namespace Managed
	{

		class ObjectHolder
		{
		public:
			ObjectHolder() : object() { }
			ObjectHolder(System::Object ^ _obj) : object(_obj) { }
			~ObjectHolder() { }
			System::Object ^ toObject()
			{
				return object;
			}
		private:
			gcroot < System::Object ^ > object;
		};


		// базовые шаблоны для конвертации переменных и типов
		template <typename T> struct Convert
		{
			typedef T Type;
			static inline Type To(T _value)
			{
				return _value;
			}
			static inline T From(Type _value)
			{
				return _value;
			}
		};

		// перегрузка для базовых типов
		template <> struct Convert<size_t>
		{
			typedef System::UInt32 Type;
			inline static System::UInt32 To(size_t _value)
			{
				return System::UInt32(_value);
			}
			inline static size_t From(System::UInt32 _value)
			{
				return size_t(_value);
			}
		};
		template <> struct Convert<bool&>
		{
			typedef bool % Type;
			inline static bool % To(bool& _value)
			{
				return reinterpret_cast<bool&>(_value);
			}
		};

		// перегрузка для строк
		template <> struct Convert<const std::string&>
		{
			typedef System::String ^ Type;
			inline static System::String ^ To(const std::string& _value)
			{
				return string_utility::utf8_to_managed(_value);
			}
			inline static std::string From(System::String ^ _value)
			{
				return string_utility::managed_to_utf8(_value);
			}
		};
		template <> struct Convert<const MyGUI::UString&>
		{
			typedef System::String ^ Type;
			inline static System::String ^ To(const MyGUI::UString& _value)
			{
				return string_utility::utf16_to_managed(_value);
			}
			inline static MyGUI::UString From(System::String ^ _value)
			{
				return string_utility::managed_to_utf16(_value);
			}
		};
		template <> struct Convert<MyGUI::UString>
		{
			typedef System::String ^ Type;
			inline static System::String ^ To(const MyGUI::UString& _value)
			{
				return string_utility::utf16_to_managed(_value);
			}
			inline static MyGUI::UString From(System::String ^ _value)
			{
				return string_utility::managed_to_utf16(_value);
			}
		};

		// прегрузка для Any
		template <> struct Convert<MyGUI::Any>
		{
			typedef System::Object ^ Type;
			inline static System::Object ^ To(MyGUI::Any _value)
			{
				ObjectHolder* data = _value.castType< ObjectHolder >(false);
				return data ? data->toObject() : nullptr;
			}
			inline static MyGUI::Any From(System::Object ^ _value)
			{
				ObjectHolder data = _value;
				return data;
			}
		};

	} // namespace Managed
} // namespace MyGUI
