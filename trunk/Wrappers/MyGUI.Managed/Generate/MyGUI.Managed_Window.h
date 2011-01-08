/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_TextBox.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class Window : public TextBox
		{

		private:
			typedef MyGUI::Window ThisType;

		public:
			Window() : TextBox() { }

		internal:
			Window( MyGUI::Window* _native ) : TextBox(_native) { }
			Window( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew Window(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint





   	public:
		property Convert<bool>::Type Movable
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getMovable() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMovable( Convert<bool>::From(_value) );
			}
		}
	


   	public:
		Convert<const MyGUI::types::TCoord< int > &>::Type GetActionScale( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::types::TCoord< int > &>::To(
				static_cast<ThisType*>(mNative)->getActionScale( ) );
		}



   	public:
		property Convert<bool>::Type Snap
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getSnap() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setSnap( Convert<bool>::From(_value) );
			}
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
		void SetCoord(
			Convert<const MyGUI::types::TCoord< int > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord(
				Convert<const MyGUI::types::TCoord< int > &>::From(_value) );
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
		void SetPosition(
			Convert<const MyGUI::types::TPoint< int > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition(
				Convert<const MyGUI::types::TPoint< int > &>::From(_value) );
		}



   	public:
		void SetMaxSize(
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaxSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}



   	public:
		property Convert<MyGUI::types::TSize< int >>::Type MaxSize
		{
			Convert<MyGUI::types::TSize< int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TSize< int >>::To( static_cast<ThisType*>(mNative)->getMaxSize() );
			}
			void set(Convert<const MyGUI::types::TSize< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMaxSize( Convert<const MyGUI::types::TSize< int > &>::From(_value) );
			}
		}
	


   	public:
		void SetMinSize(
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMinSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}



   	public:
		property Convert<MyGUI::types::TSize< int >>::Type MinSize
		{
			Convert<MyGUI::types::TSize< int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TSize< int >>::To( static_cast<ThisType*>(mNative)->getMinSize() );
			}
			void set(Convert<const MyGUI::types::TSize< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMinSize( Convert<const MyGUI::types::TSize< int > &>::From(_value) );
			}
		}
	


   	public:
		Convert<MyGUI::TextBox *>::Type GetCaptionWidget( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::TextBox *>::To(
				static_cast<ThisType*>(mNative)->getCaptionWidget( ) );
		}



   	public:
		property Convert<const MyGUI::UString &>::Type Caption
		{
			Convert<const MyGUI::UString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::UString &>::To( static_cast<ThisType*>(mNative)->getCaption() );
			}
			void set(Convert<const MyGUI::UString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCaption( Convert<const MyGUI::UString &>::From(_value) );
			}
		}
	


   	public:
		property Convert<bool>::Type AutoAlpha
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getAutoAlpha() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAutoAlpha( Convert<bool>::From(_value) );
			}
		}
	


   	public:
		void DestroySmooth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->destroySmooth( );
		}



   	public:
		void SetVisibleSmooth(
			Convert<bool>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setVisibleSmooth(
				Convert<bool>::From(_value) );
		}



   	public:
		void SetVisible(
			Convert<bool>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setVisible(
				Convert<bool>::From(_value) );
		}



   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
