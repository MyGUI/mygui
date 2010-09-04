/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_Widget.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class StaticText : public Widget
		{

		private:
			typedef MyGUI::StaticText ThisType;

		public:
			StaticText() : Widget() { }

		internal:
			StaticText( MyGUI::StaticText* _native ) : Widget(_native) { }
			StaticText( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew StaticText(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   	public:
		void SetProperty(
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setProperty(
				Convert<const std::string &>::From(_key) ,
				Convert<const std::string &>::From(_value) );
		}



   	public:
		property Convert<const MyGUI::Colour &>::Type TextColour
		{
			Convert<const MyGUI::Colour &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::Colour &>::To( static_cast<ThisType*>(mNative)->getTextColour() );
			}
			void set(Convert<const MyGUI::Colour &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTextColour( Convert<const MyGUI::Colour &>::From(_value) );
			}
		}
	


   	public:
		property Convert<MyGUI::Align>::Type TextAlign
		{
			Convert<MyGUI::Align>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::Align>::To( static_cast<ThisType*>(mNative)->getTextAlign() );
			}
			void set(Convert<MyGUI::Align>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTextAlign( Convert<MyGUI::Align>::From(_value) );
			}
		}
	


   	public:
		property Convert<int>::Type FontHeight
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getFontHeight() );
			}
			void set(Convert<int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setFontHeight( Convert<int>::From(_value) );
			}
		}
	


   	public:
		property Convert<const std::string &>::Type FontName
		{
			Convert<const std::string &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getFontName() );
			}
			void set(Convert<const std::string &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setFontName( Convert<const std::string &>::From(_value) );
			}
		}
	


   	public:
		Convert<MyGUI::types::TSize< int >>::Type GetTextSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::types::TSize< int >>::To(
				static_cast<ThisType*>(mNative)->getTextSize( ) );
		}



   	public:
		Convert<MyGUI::types::TCoord< int >>::Type GetTextRegion( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::types::TCoord< int >>::To(
				static_cast<ThisType*>(mNative)->getTextRegion( ) );
		}



   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
