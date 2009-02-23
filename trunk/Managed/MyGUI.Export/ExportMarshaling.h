/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __EXPORT_MARSHALING_H__
#define __EXPORT_MARSHALING_H__

#include <MyGUI.h>
#include "ExportDefine.h"

namespace Export
{

	// базовые шаблоны для конвертации переменных и типов
	template <typename T> struct Convert
	{
		typedef T Type;
		static inline Type To(T _value) { return _value; }
		static inline T From(Type _value) { return _value; }
	};


	template <> struct Convert< MyGUI::IntSize >
	{
		typedef const MyGUI::IntSize& Type;
		static MyGUI::IntSize mHolder;
		inline static const MyGUI::IntSize& To(const MyGUI::IntSize& _value) { mHolder = _value; return mHolder; }
		inline static const MyGUI::IntSize& From(const MyGUI::IntSize& _value) { return _value; }
	};

	template <> struct Convert< MyGUI::IntPoint >
	{
		typedef const MyGUI::IntPoint& Type;
		static MyGUI::IntPoint mHolder;
		inline static const MyGUI::IntPoint& To(const MyGUI::IntPoint& _value) { mHolder = _value; return mHolder; }
		inline static const MyGUI::IntPoint& From(const MyGUI::IntPoint& _value) { return _value; }
	};

	template <> struct Convert< MyGUI::IntCoord >
	{
		typedef const MyGUI::IntCoord& Type;
		static MyGUI::IntCoord mHolder;
		inline static const MyGUI::IntCoord& To(const MyGUI::IntCoord& _value) { mHolder = _value; return mHolder; }
		inline static const MyGUI::IntCoord& From(const MyGUI::IntCoord& _value) { return _value; }
	};

	template <> struct Convert< MyGUI::IntRect >
	{
		typedef const MyGUI::IntRect& Type;
		static MyGUI::IntRect mHolder;
		inline static const MyGUI::IntRect& To(const MyGUI::IntRect& _value) { mHolder = _value; return mHolder; }
		inline static const MyGUI::IntRect& From(const MyGUI::IntRect& _value) { return _value; }
	};

	template <> struct Convert< MyGUI::FloatSize >
	{
		typedef const MyGUI::FloatSize& Type;
		static MyGUI::FloatSize mHolder;
		inline static const MyGUI::FloatSize& To(const MyGUI::FloatSize& _value) { mHolder = _value; return mHolder; }
		inline static const MyGUI::FloatSize& From(const MyGUI::FloatSize& _value) { return _value; }
	};

	template <> struct Convert< MyGUI::FloatPoint >
	{
		typedef const MyGUI::FloatPoint& Type;
		static MyGUI::FloatPoint mHolder;
		inline static const MyGUI::FloatPoint& To(const MyGUI::FloatPoint& _value) { mHolder = _value; return mHolder; }
		inline static const MyGUI::FloatPoint& From(const MyGUI::FloatPoint& _value) { return _value; }
	};

	template <> struct Convert< MyGUI::FloatCoord >
	{
		typedef const MyGUI::FloatCoord& Type;
		static MyGUI::FloatCoord mHolder;
		inline static const MyGUI::FloatCoord& To(const MyGUI::FloatCoord& _value) { mHolder = _value; return mHolder; }
		inline static const MyGUI::FloatCoord& From(const MyGUI::FloatCoord& _value) { return _value; }
	};

	template <> struct Convert< MyGUI::FloatRect >
	{
		typedef const MyGUI::FloatRect& Type;
		static MyGUI::FloatRect mHolder;
		inline static const MyGUI::FloatRect& To(const MyGUI::FloatRect& _value) { mHolder = _value; return mHolder; }
		inline static const MyGUI::FloatRect& From(const MyGUI::FloatRect& _value) { return _value; }
	};


	template <> struct Convert< MyGUI::Colour >
	{
		typedef const MyGUI::Colour& Type;
		static MyGUI::Colour mHolder;
		inline static const MyGUI::Colour& To(const MyGUI::Colour& _value) { mHolder = _value; return mHolder; }
		inline static const MyGUI::Colour& From(const MyGUI::Colour& _value) { return _value; }
	};

	template <> struct Convert< Ogre::UTFString >
	{
		typedef const wchar_t* Type;
		static Ogre::UTFString mHolder;
		inline static const wchar_t* To(const Ogre::UTFString& _value) { mHolder = _value; return mHolder.asWStr_c_str(); }
		inline static Ogre::UTFString From(const wchar_t* _value) { return _value; }
	};

	template <> struct Convert< std::string >
	{
		typedef const char* Type;
		static std::string mHolder;
		inline static const char* To(const std::string& _value) { mHolder = _value; return mHolder.c_str(); }
		inline static std::string From(const char* _value) { return _value; }
	};

	template <> struct Convert< const Ogre::UTFString& >
	{
		typedef const wchar_t* Type;
		inline static const wchar_t* To(const Ogre::UTFString& _value) { return _value.asWStr_c_str(); }
		inline static Ogre::UTFString From(const wchar_t* _value) { return _value; }
	};

	template <> struct Convert< const std::string& >
	{
		typedef const char* Type;
		inline static const char* To(const std::string& _value) { return _value.c_str(); }
		inline static std::string From(const char* _value) { return _value; }
	};

	template <> struct Convert< MyGUI::Align >
	{
		typedef int Type;
		inline static int To(MyGUI::Align _value) { return *((int*)(&_value)); }
		inline static MyGUI::Align From(int _value) { return MyGUI::Align::Enum(_value); }
	};

	template <> struct Convert< MyGUI::DDItemState >
	{
		typedef int Type;
		inline static int To(MyGUI::DDItemState _value) { return *((int*)(&_value)); }
		inline static MyGUI::DDItemState From(int _value) { return MyGUI::DDItemState::Enum(_value); }
	};

	template <> struct Convert< MyGUI::KeyCode >
	{
		typedef int Type;
		inline static int To(MyGUI::KeyCode _value) { return *((int*)(&_value)); }
		inline static MyGUI::KeyCode From(int _value) { return MyGUI::KeyCode::Enum(_value); }
	};

	template <> struct Convert< MyGUI::MenuItemType >
	{
		typedef int Type;
		inline static int To(MyGUI::MenuItemType _value) { return *((int*)(&_value)); }
		inline static MyGUI::MenuItemType From(int _value) { return MyGUI::MenuItemType::Enum(_value); }
	};

	template <> struct Convert< MyGUI::MessageBoxStyle >
	{
		typedef int Type;
		inline static int To(MyGUI::MessageBoxStyle _value) { return *((int*)(&_value)); }
		inline static MyGUI::MessageBoxStyle From(int _value) { return MyGUI::MessageBoxStyle::Enum(_value); }
	};

	template <> struct Convert< MyGUI::MouseButton >
	{
		typedef int Type;
		inline static int To(MyGUI::MouseButton _value) { return *((int*)(&_value)); }
		inline static MyGUI::MouseButton From(int _value) { return MyGUI::MouseButton::Enum(_value); }
	};

	template <> struct Convert< MyGUI::WidgetStyle >
	{
		typedef int Type;
		inline static int To(MyGUI::WidgetStyle _value) { return *((int*)(&_value)); }
		inline static MyGUI::WidgetStyle From(int _value) { return MyGUI::WidgetStyle::Enum(_value); }
	};

	template <> struct Convert< MyGUI::Any >
	{
		typedef IUnknown Type;
		inline static IUnknown To(MyGUI::Any& _value)
		{
			IUnknown* data = _value.castType<IUnknown>(false);
			return data == nullptr ? nullptr : *data;
		}
		inline static MyGUI::Any From(IUnknown _value) { return _value;	 }
	};

} // namespace Export

#endif // __EXPORT_MARSHALING_H__
