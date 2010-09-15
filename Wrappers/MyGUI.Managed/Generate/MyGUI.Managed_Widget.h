/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_WidgetEvent.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class Widget : public WidgetEvent
		{

		private:
			typedef MyGUI::Widget ThisType;

		public:
			Widget() : WidgetEvent() { }

		internal:
			Widget( MyGUI::Widget* _native ) : WidgetEvent(_native) { }
			Widget( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew Widget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   


   	public:
		void ChangeWidgetSkin(
			Convert<const std::string &>::Type _skinname )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->changeWidgetSkin(
				Convert<const std::string &>::From(_skinname) );
		}



   	public:
		property Convert<bool>::Type EnableToolTip
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getEnableToolTip() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEnableToolTip( Convert<bool>::From(_value) );
			}
		}
	


   


   


   	public:
		void SetEnabledSilent(
			Convert<bool>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setEnabledSilent(
				Convert<bool>::From(_value) );
		}



   	public:
		void SetMaskPick(
			Convert<const std::string &>::Type _filename )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaskPick(
				Convert<const std::string &>::From(_filename) );
		}



   	public:
		property Convert<bool>::Type InheritsPick
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isInheritsPick() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setInheritsPick( Convert<bool>::From(_value) );
			}
		}
	


   	public:
		Convert<MyGUI::Widget *>::Type GetChildAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::Widget *>::To(
				static_cast<ThisType*>(mNative)->getChildAt(
					Convert<size_t>::From(_index) ) );
		}



   	public:
		Convert<size_t>::Type GetChildCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getChildCount( ) );
		}



   


   	public:
		Convert<MyGUI::Widget *>::Type GetParent( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::Widget *>::To(
				static_cast<ThisType*>(mNative)->getParent( ) );
		}



   	public:
		property Convert<float>::Type Alpha
		{
			Convert<float>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<float>::To( static_cast<ThisType*>(mNative)->getAlpha() );
			}
			void set(Convert<float>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAlpha( Convert<float>::From(_value) );
			}
		}
	


   	public:
		void SetRealCoord(
			Convert<float>::Type _left ,
			Convert<float>::Type _top ,
			Convert<float>::Type _width ,
			Convert<float>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealCoord(
				Convert<float>::From(_left) ,
				Convert<float>::From(_top) ,
				Convert<float>::From(_width) ,
				Convert<float>::From(_height) );
		}



   	public:
		void SetRealSize(
			Convert<float>::Type _width ,
			Convert<float>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealSize(
				Convert<float>::From(_width) ,
				Convert<float>::From(_height) );
		}



   	public:
		void SetCoord(
			Convert<int>::Type _left ,
			Convert<int>::Type _top ,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord(
				Convert<int>::From(_left) ,
				Convert<int>::From(_top) ,
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}



   	public:
		void SetSize(
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}



   	public:
		void SetPosition(
			Convert<int>::Type _left ,
			Convert<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition(
				Convert<int>::From(_left) ,
				Convert<int>::From(_top) );
		}



   	public:
		void SetSize(
			Convert<const MyGUI::types::TSize< int > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize(
				Convert<const MyGUI::types::TSize< int > &>::From(_value) );
		}



   	public:
		Convert<const std::string &>::Type GetName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To(
				static_cast<ThisType*>(mNative)->getName( ) );
		}



   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
