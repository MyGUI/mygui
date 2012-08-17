/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#ifndef _0981cba7_579a_4c4d_b104_7ea84cb8f263_
#define _0981cba7_579a_4c4d_b104_7ea84cb8f263_

#include <MyGUI.h>
#include "ExportDefine.h"

namespace Export
{

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


	template <> struct Convert<MyGUI::IntSize>
	{
		typedef const MyGUI::IntSize& Type;
		static MyGUI::IntSize mHolder;
		inline static Type To(Type _value)
		{
			mHolder = _value;
			return mHolder;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<MyGUI::IntPoint>
	{
		typedef const MyGUI::IntPoint& Type;
		static MyGUI::IntPoint mHolder;
		inline static const Type To(Type _value)
		{
			mHolder = _value;
			return mHolder;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<MyGUI::IntCoord>
	{
		typedef const MyGUI::IntCoord& Type;
		static MyGUI::IntCoord mHolder;
		inline static Type To(Type _value)
		{
			mHolder = _value;
			return mHolder;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<MyGUI::IntRect>
	{
		typedef const MyGUI::IntRect& Type;
		static MyGUI::IntRect mHolder;
		inline static Type To(Type _value)
		{
			mHolder = _value;
			return mHolder;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<MyGUI::FloatSize>
	{
		typedef const MyGUI::FloatSize& Type;
		static MyGUI::FloatSize mHolder;
		inline static Type To(Type _value)
		{
			mHolder = _value;
			return mHolder;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<MyGUI::FloatPoint>
	{
		typedef const MyGUI::FloatPoint& Type;
		static MyGUI::FloatPoint mHolder;
		inline static const Type To(Type _value)
		{
			mHolder = _value;
			return mHolder;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<MyGUI::FloatCoord>
	{
		typedef const MyGUI::FloatCoord& Type;
		static MyGUI::FloatCoord mHolder;
		inline static Type To(Type _value)
		{
			mHolder = _value;
			return mHolder;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<MyGUI::FloatRect>
	{
		typedef const MyGUI::FloatRect& Type;
		static MyGUI::FloatRect mHolder;
		inline static Type To(Type _value)
		{
			mHolder = _value;
			return mHolder;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};


	template <> struct Convert<MyGUI::Colour>
	{
		typedef const MyGUI::Colour& Type;
		static MyGUI::Colour mHolder;
		inline static Type To(Type _value)
		{
			mHolder = _value;
			return mHolder;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<MyGUI::UString>
	{
		typedef const wchar_t* Type;
		static MyGUI::UString mHolder;
		inline static Type To(const MyGUI::UString& _value)
		{
			mHolder = _value;
			return mHolder.asWStr_c_str();
		}
		inline static MyGUI::UString From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<std::string>
	{
		typedef const char* Type;
		static std::string mHolder;
		inline static Type To(const std::string& _value)
		{
			mHolder = _value;
			return mHolder.c_str();
		}
		inline static std::string From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<const MyGUI::UString&>
	{
		typedef const wchar_t* Type;
		inline static Type To(const MyGUI::UString& _value)
		{
			return _value.asWStr_c_str();
		}
		inline static MyGUI::UString From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<const std::string&>
	{
		typedef const char* Type;
		inline static Type To(const std::string& _value)
		{
			return _value.c_str();
		}
		inline static std::string From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<MyGUI::Align>
	{
		typedef int Type;
		inline static Type To(MyGUI::Align _value)
		{
			return _value.getValue();
		}
		inline static MyGUI::Align From(Type _value)
		{
			return MyGUI::Align::Enum(_value);
		}
	};

	template <> struct Convert<MyGUI::DDItemState>
	{
		typedef int Type;
		inline static Type To(MyGUI::DDItemState _value)
		{
			return _value.getValue();
		}
		inline static MyGUI::DDItemState From(Type _value)
		{
			return MyGUI::DDItemState::Enum(_value);
		}
	};

	template <> struct Convert<MyGUI::KeyCode>
	{
		typedef int Type;
		inline static Type To(MyGUI::KeyCode _value)
		{
			return _value.getValue();
		}
		inline static MyGUI::KeyCode From(Type _value)
		{
			return MyGUI::KeyCode::Enum(_value);
		}
	};

	template <> struct Convert<MyGUI::MenuItemType>
	{
		typedef int Type;
		inline static Type To(MyGUI::MenuItemType _value)
		{
			return _value.getValue();
		}
		inline static MyGUI::MenuItemType From(Type _value)
		{
			return MyGUI::MenuItemType::Enum(_value);
		}
	};

	template <> struct Convert<MyGUI::FlowDirection>
	{
		typedef int Type;
		inline static Type To(MyGUI::FlowDirection _value)
		{
			return _value.getValue();
		}
		inline static MyGUI::FlowDirection From(Type _value)
		{
			return MyGUI::FlowDirection::Enum(_value);
		}
	};

	template <> struct Convert<MyGUI::ResizingPolicy>
	{
		typedef int Type;
		inline static Type To(MyGUI::ResizingPolicy _value)
		{
			return _value.getValue();
		}
		inline static MyGUI::ResizingPolicy From(Type _value)
		{
			return MyGUI::ResizingPolicy::Enum(_value);
		}
	};

	template <> struct Convert<MyGUI::MouseButton>
	{
		typedef int Type;
		inline static Type To(MyGUI::MouseButton _value)
		{
			return _value.getValue();
		}
		inline static MyGUI::MouseButton From(Type _value)
		{
			return MyGUI::MouseButton::Enum(_value);
		}
	};

	template <> struct Convert<MyGUI::WidgetStyle>
	{
		typedef int Type;
		inline static Type To(MyGUI::WidgetStyle _value)
		{
			return _value.getValue();
		}
		inline static MyGUI::WidgetStyle From(Type _value)
		{
			return MyGUI::WidgetStyle::Enum(_value);
		}
	};

	template <> struct Convert<MyGUI::Any>
	{
		typedef IUnknown Type;
		inline static Type To(MyGUI::Any& _value)
		{
			IUnknown* data = _value.castType<IUnknown>(false);
			return data == nullptr ? nullptr : *data;
		}
		inline static MyGUI::Any From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<MyGUI::LogLevel>
	{
		typedef int Type;
		inline static Type To(MyGUI::LogLevel _value)
		{
			return _value.getValue();
		}
		inline static MyGUI::LogLevel From(Type _value)
		{
			return MyGUI::LogLevel::Enum(_value);
		}
	};

	template <> struct Convert<void*&>
	{
		typedef void*& Type;
		inline static Type To(Type _value)
		{
			return _value;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<int&>
	{
		typedef int& Type;
		inline static Type To(Type _value)
		{
			return _value;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<size_t&>
	{
		typedef size_t& Type;
		inline static Type To(Type _value)
		{
			return _value;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

	template <> struct Convert<bool&>
	{
		typedef bool& Type;
		inline static Type To(Type _value)
		{
			return _value;
		}
		inline static Type From(Type _value)
		{
			return _value;
		}
	};

}

#endif
