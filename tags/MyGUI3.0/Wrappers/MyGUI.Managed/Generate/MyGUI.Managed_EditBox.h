/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_StaticText.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class EditBox : public StaticText
		{

		private:
			typedef MyGUI::Edit ThisType;

		public:
			EditBox() : StaticText() { }

		internal:
			EditBox( MyGUI::Edit* _native ) : StaticText(_native) { }
			EditBox( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew EditBox(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   	public:
		delegate void HandleEditTextChange(
			Convert<MyGUI::Edit *>::Type _sender );
		event HandleEditTextChange^ EventEditTextChange
		{
			void add(HandleEditTextChange^ _value)
			{
				mDelegateEditTextChange += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventEditTextChange =
					static_cast< MyGUI::delegates::IDelegate1<
						MyGUI::Edit * > *>(
							new Delegate1< HandleEditTextChange^ ,
							MyGUI::Edit * >(mDelegateEditTextChange) );
			}
			void remove(HandleEditTextChange^ _value)
			{
				mDelegateEditTextChange -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateEditTextChange == nullptr)
					static_cast<ThisType*>(mNative)->eventEditTextChange = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventEditTextChange =
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::Edit * > *>(
								new Delegate1< HandleEditTextChange^ ,
									MyGUI::Edit * >(mDelegateEditTextChange) );
			}
		}
	private:
		HandleEditTextChange^ mDelegateEditTextChange;



   	public:
		delegate void HandleEditSelectAccept(
			Convert<MyGUI::Edit *>::Type _sender );
		event HandleEditSelectAccept^ EventEditSelectAccept
		{
			void add(HandleEditSelectAccept^ _value)
			{
				mDelegateEditSelectAccept += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventEditSelectAccept =
					static_cast< MyGUI::delegates::IDelegate1<
						MyGUI::Edit * > *>(
							new Delegate1< HandleEditSelectAccept^ ,
							MyGUI::Edit * >(mDelegateEditSelectAccept) );
			}
			void remove(HandleEditSelectAccept^ _value)
			{
				mDelegateEditSelectAccept -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateEditSelectAccept == nullptr)
					static_cast<ThisType*>(mNative)->eventEditSelectAccept = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventEditSelectAccept =
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::Edit * > *>(
								new Delegate1< HandleEditSelectAccept^ ,
									MyGUI::Edit * >(mDelegateEditSelectAccept) );
			}
		}
	private:
		HandleEditSelectAccept^ mDelegateEditSelectAccept;



   


   


   	public:
		property Convert<size_t>::Type HScrollPosition
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getHScrollPosition() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setHScrollPosition( Convert<size_t>::From(_value) );
			}
		}
	


   	public:
		Convert<size_t>::Type GetHScrollRange( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getHScrollRange( ) );
		}



   	public:
		Convert<size_t>::Type GetVScrollRange( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getVScrollRange( ) );
		}



   


   


   


   


   	public:
		property Convert<bool>::Type InvertSelected
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getInvertSelected() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setInvertSelected( Convert<bool>::From(_value) );
			}
		}
	


   	public:
		property Convert<unsigned int>::Type PasswordChar
		{
			Convert<unsigned int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<unsigned int>::To( static_cast<ThisType*>(mNative)->getPasswordChar() );
			}
			void set(Convert<unsigned int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPasswordChar( Convert<unsigned int>::From(_value) );
			}
		}
	


   	public:
		property Convert<bool>::Type EditPassword
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getEditPassword() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEditPassword( Convert<bool>::From(_value) );
			}
		}
	


   	public:
		property Convert<bool>::Type EditReadOnly
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getEditReadOnly() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEditReadOnly( Convert<bool>::From(_value) );
			}
		}
	


   


   	public:
		property Convert<size_t>::Type TextCursor
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getTextCursor() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTextCursor( Convert<size_t>::From(_value) );
			}
		}
	


   	public:
		Convert<bool>::Type IsTextSelection( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->isTextSelection( ) );
		}



   	public:
		Convert<MyGUI::UString>::Type GetTextSelection( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::UString>::To(
				static_cast<ThisType*>(mNative)->getTextSelection( ) );
		}



   	public:
		void SetTextSelection(
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _end )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextSelection(
				Convert<size_t>::From(_start) ,
				Convert<size_t>::From(_end) );
		}



   	public:
		Convert<MyGUI::UString>::Type GetTextInterval(
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _count )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::UString>::To(
				static_cast<ThisType*>(mNative)->getTextInterval(
					Convert<size_t>::From(_start) ,
					Convert<size_t>::From(_count) ) );
		}



   	public:
		Convert<size_t>::Type GetTextSelectionLength( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getTextSelectionLength( ) );
		}



   	public:
		Convert<size_t>::Type GetTextSelectionEnd( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getTextSelectionEnd( ) );
		}



   	public:
		void SetTextIntervalColour(
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _count ,
			Convert<const MyGUI::Colour &>::Type _colour )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextIntervalColour(
				Convert<size_t>::From(_start) ,
				Convert<size_t>::From(_count) ,
				Convert<const MyGUI::Colour &>::From(_colour) );
		}



   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
