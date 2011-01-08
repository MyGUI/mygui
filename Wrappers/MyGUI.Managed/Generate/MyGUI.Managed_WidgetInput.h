/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "../BaseWidget.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class WidgetInput abstract : public BaseWidget
		{

		private:
			typedef MyGUI::Widget ThisType;

		public:
			WidgetInput() : BaseWidget() { }

		internal:
			WidgetInput( MyGUI::Widget* _native ) : BaseWidget(_native) { }

			//InsertPoint

































   	public:
		Convert<bool>::Type GetRootKeyFocus( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->getRootKeyFocus( ) );
		}



   	public:
		Convert<bool>::Type GetRootMouseFocus( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->getRootMouseFocus( ) );
		}



   	public:
		Convert<bool>::Type IsMaskPickInside(
			Convert<const MyGUI::types::TPoint< int > &>::Type _point ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->isMaskPickInside(
					Convert<const MyGUI::types::TPoint< int > &>::From(_point) ,
					Convert<const MyGUI::types::TCoord< int > &>::From(_coord) ) );
		}



   	public:
		void SetMaskPick(
			Convert<const MyGUI::MaskPickInfo &>::Type _info )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaskPick(
				Convert<const MyGUI::MaskPickInfo &>::From(_info) );
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
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getInheritsPick() );
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
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getNeedMouseFocus() );
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
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getNeedKeyFocus() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedKeyFocus( Convert<bool>::From(_value) );
			}
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
	


		};

	} // namespace Managed
} // namespace MyGUI
