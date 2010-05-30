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
		void UpdateArrange(
			Convert<const MyGUI::types::TCoord< int > &>::Type _coordPlace ,
			Convert<const MyGUI::types::TSize< int > &>::Type _oldsize )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->updateArrange(
				Convert<const MyGUI::types::TCoord< int > &>::From(_coordPlace) ,
				Convert<const MyGUI::types::TSize< int > &>::From(_oldsize) );
		}



   	public:
		Convert<int>::Type GetPaddingHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getPaddingHeight( ) );
		}



   	public:
		Convert<int>::Type GetPaddingWidth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getPaddingWidth( ) );
		}



   	public:
		Convert<int>::Type GetMarginHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getMarginHeight( ) );
		}



   	public:
		Convert<int>::Type GetMarginWidth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To(
				static_cast<ThisType*>(mNative)->getMarginWidth( ) );
		}



   	public:
		Convert<const MyGUI::types::TSize< int > &>::Type GetDesiredSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::types::TSize< int > &>::To(
				static_cast<ThisType*>(mNative)->getDesiredSize( ) );
		}



   	public:
		void UpdateMeasure(
			Convert<const MyGUI::types::TSize< int > &>::Type _sizeAvailable )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->updateMeasure(
				Convert<const MyGUI::types::TSize< int > &>::From(_sizeAvailable) );
		}



   	public:
		void InvalidateMeasure( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->invalidateMeasure( );
		}



   	public:
		void SetCaptionWithNewLine(
			Convert<const std::string &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCaptionWithNewLine(
				Convert<const std::string &>::From(_value) );
		}



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
		property Convert<bool>::Type NeedToolTip
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getNeedToolTip() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedToolTip( Convert<bool>::From(_value) );
			}
		}
	


   


   


   	public:
		Convert<MyGUI::Widget *>::Type GetClientWidget( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::Widget *>::To(
				static_cast<ThisType*>(mNative)->getClientWidget( ) );
		}



   	public:
		Convert<MyGUI::types::TCoord< int >>::Type GetClientCoord( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::types::TCoord< int >>::To(
				static_cast<ThisType*>(mNative)->getClientCoord( ) );
		}



   


   	public:
		property Convert<const std::string &>::Type Pointer
		{
			Convert<const std::string &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getPointer() );
			}
			void set(Convert<const std::string &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPointer( Convert<const std::string &>::From(_value) );
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
		property Convert<bool>::Type Enabled
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isEnabled() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEnabled( Convert<bool>::From(_value) );
			}
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
		property Convert<bool>::Type NeedMouseFocus
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isNeedMouseFocus() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedMouseFocus( Convert<bool>::From(_value) );
			}
		}
	


   	public:
		property Convert<bool>::Type NeedKeyFocus
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isNeedKeyFocus() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedKeyFocus( Convert<bool>::From(_value) );
			}
		}
	


   	public:
		Convert<MyGUI::Widget *>::Type FindWidget(
			Convert<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::Widget *>::To(
				static_cast<ThisType*>(mNative)->findWidget(
					Convert<const std::string &>::From(_name) ) );
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
		Convert<MyGUI::types::TSize< int >>::Type GetParentSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::types::TSize< int >>::To(
				static_cast<ThisType*>(mNative)->getParentSize( ) );
		}



   	public:
		Convert<MyGUI::Widget *>::Type GetVisualParent( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::Widget *>::To(
				static_cast<ThisType*>(mNative)->getVisualParent( ) );
		}



   	public:
		Convert<MyGUI::Widget *>::Type GetParent( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::Widget *>::To(
				static_cast<ThisType*>(mNative)->getParent( ) );
		}



   	public:
		Convert<bool>::Type IsRootWidget( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->isRootWidget( ) );
		}



   	public:
		void SetColour(
			Convert<const MyGUI::Colour &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColour(
				Convert<const MyGUI::Colour &>::From(_value) );
		}



   	public:
		Convert<bool>::Type SetState(
			Convert<const std::string &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->setState(
					Convert<const std::string &>::From(_value) ) );
		}



   	public:
		property Convert<MyGUI::SizePolicy>::Type SizePolicy
		{
			Convert<MyGUI::SizePolicy>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::SizePolicy>::To( static_cast<ThisType*>(mNative)->getSizePolicy() );
			}
			void set(Convert<MyGUI::SizePolicy>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setSizePolicy( Convert<MyGUI::SizePolicy>::From(_value) );
			}
		}
	


   	public:
		property Convert<const MyGUI::types::TSize< int > &>::Type MaxSize
		{
			Convert<const MyGUI::types::TSize< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TSize< int > &>::To( static_cast<ThisType*>(mNative)->getMaxSize() );
			}
			void set(Convert<const MyGUI::types::TSize< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMaxSize( Convert<const MyGUI::types::TSize< int > &>::From(_value) );
			}
		}
	


   	public:
		property Convert<const MyGUI::types::TSize< int > &>::Type MinSize
		{
			Convert<const MyGUI::types::TSize< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TSize< int > &>::To( static_cast<ThisType*>(mNative)->getMinSize() );
			}
			void set(Convert<const MyGUI::types::TSize< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMinSize( Convert<const MyGUI::types::TSize< int > &>::From(_value) );
			}
		}
	


   	public:
		property Convert<const MyGUI::types::TRect< int > &>::Type Padding
		{
			Convert<const MyGUI::types::TRect< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TRect< int > &>::To( static_cast<ThisType*>(mNative)->getPadding() );
			}
			void set(Convert<const MyGUI::types::TRect< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPadding( Convert<const MyGUI::types::TRect< int > &>::From(_value) );
			}
		}
	


   	public:
		property Convert<const MyGUI::types::TRect< int > &>::Type Margin
		{
			Convert<const MyGUI::types::TRect< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TRect< int > &>::To( static_cast<ThisType*>(mNative)->getMargin() );
			}
			void set(Convert<const MyGUI::types::TRect< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMargin( Convert<const MyGUI::types::TRect< int > &>::From(_value) );
			}
		}
	


   	public:
		property Convert<bool>::Type InheritsAlpha
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isInheritsAlpha() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setInheritsAlpha( Convert<bool>::From(_value) );
			}
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
		void SetAlign(
			Convert<MyGUI::Align>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setAlign(
				Convert<MyGUI::Align>::From(_value) );
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
		void SetRealPosition(
			Convert<float>::Type _left ,
			Convert<float>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealPosition(
				Convert<float>::From(_left) ,
				Convert<float>::From(_top) );
		}



   	public:
		void SetRealCoord(
			Convert<const MyGUI::types::TCoord< float > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealCoord(
				Convert<const MyGUI::types::TCoord< float > &>::From(_value) );
		}



   	public:
		void SetRealSize(
			Convert<const MyGUI::types::TSize< float > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealSize(
				Convert<const MyGUI::types::TSize< float > &>::From(_value) );
		}



   	public:
		void SetRealPosition(
			Convert<const MyGUI::types::TPoint< float > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealPosition(
				Convert<const MyGUI::types::TPoint< float > &>::From(_value) );
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
		Convert<const std::string &>::Type GetName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To(
				static_cast<ThisType*>(mNative)->getName( ) );
		}



   


   


   


   


   


   


   


   


   


   


   


   


   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
