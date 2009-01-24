/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"


namespace MMyGUI
{

	public ref class Edit : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( Edit, Widget );

		//InsertPoint

   	public:
		delegate void HandleEditTextChange( Convert<MyGUI::Widget *>::Type _sender );
		event HandleEditTextChange^ EditTextChange
		{
			void add(HandleEditTextChange^ _value)
			{
				mDelegateEditTextChange += _value;
				static_cast<ThisType*>(mNative)->eventEditTextChange = new Delegate1< HandleEditTextChange^, MyGUI::Widget * >(mDelegateEditTextChange);
			}
			void remove(HandleEditTextChange^ _value)
			{
				mDelegateEditTextChange -= _value;
				if (mDelegateEditTextChange == nullptr)
					static_cast<ThisType*>(mNative)->eventEditTextChange = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventEditTextChange = new Delegate1< HandleEditTextChange^, MyGUI::Widget * >(mDelegateEditTextChange);
			}
		}
	private:
		HandleEditTextChange^ mDelegateEditTextChange;




   	public:
		delegate void HandleEditSelectAccept( Convert<MyGUI::Widget *>::Type _sender );
		event HandleEditSelectAccept^ EditSelectAccept
		{
			void add(HandleEditSelectAccept^ _value)
			{
				mDelegateEditSelectAccept += _value;
				static_cast<ThisType*>(mNative)->eventEditSelectAccept = new Delegate1< HandleEditSelectAccept^, MyGUI::Widget * >(mDelegateEditSelectAccept);
			}
			void remove(HandleEditSelectAccept^ _value)
			{
				mDelegateEditSelectAccept -= _value;
				if (mDelegateEditSelectAccept == nullptr)
					static_cast<ThisType*>(mNative)->eventEditSelectAccept = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventEditSelectAccept = new Delegate1< HandleEditSelectAccept^, MyGUI::Widget * >(mDelegateEditSelectAccept);
			}
		}
	private:
		HandleEditSelectAccept^ mDelegateEditSelectAccept;




   	public:
		property bool VisibleHScroll
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isVisibleHScroll( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVisibleHScroll(_value);
			}
		}



   	public:
		property bool VisibleVScroll
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isVisibleVScroll( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVisibleVScroll(_value);
			}
		}



   	public:
		property Convert<bool>::Type TabPrinting
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getTabPrinting() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTabPrinting( Convert<bool>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type EditWordWrap
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getEditWordWrap() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEditWordWrap( Convert<bool>::From(_value) );
			}
		}



   	public:
		void SetPasswordChar( Convert<const Ogre::UTFString &>::Type _char )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPasswordChar( Convert<const Ogre::UTFString &>::From(_char) );
		}



   	public:
		property Convert< unsigned int >::Type PasswordChar
		{
			Convert< unsigned int >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert< unsigned int >::To( static_cast<ThisType*>(mNative)->getPasswordChar() );
			}
			void set(Convert< unsigned int >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPasswordChar( Convert< unsigned int >::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type EditStatic
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getEditStatic() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEditStatic( Convert<bool>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type EditMultiLine
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getEditMultiLine() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEditMultiLine( Convert<bool>::From(_value) );
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
		void SetTextSelectColour( Convert<const MyGUI::Colour &>::Type _colour )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextSelectColour( Convert<const MyGUI::Colour &>::From(_colour) );
		}



   	public:
		void EraseText( Convert<size_t>::Type _start, Convert<size_t>::Type _count )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->eraseText( Convert<size_t>::From(_start), Convert<size_t>::From(_count) );
		}


   	public:
		void EraseText( Convert<size_t>::Type _start )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->eraseText( Convert<size_t>::From(_start) );
		}



   	public:
		void AddText( Convert<const Ogre::UTFString &>::Type _text )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addText( Convert<const Ogre::UTFString &>::From(_text) );
		}



   	public:
		void InsertText( Convert<const Ogre::UTFString &>::Type _text, Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertText( Convert<const Ogre::UTFString &>::From(_text), Convert<size_t>::From(_index) );
		}


   	public:
		void InsertText( Convert<const Ogre::UTFString &>::Type _text )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertText( Convert<const Ogre::UTFString &>::From(_text) );
		}



   	public:
		Convert<bool>::Type DeleteTextSelect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->deleteTextSelect( ) );
		}



   	public:
		Convert<Ogre::UTFString>::Type GetText( Convert<size_t>::Type _start, Convert<size_t>::Type _count )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<Ogre::UTFString>::To( static_cast<ThisType*>(mNative)->getText( Convert<size_t>::From(_start), Convert<size_t>::From(_count) ) );
		}



   	public:
		property Convert<size_t>::Type MaxTextLength
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getMaxTextLength() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMaxTextLength( Convert<size_t>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type OverflowToTheLeft
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getOverflowToTheLeft() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setOverflowToTheLeft( Convert<bool>::From(_value) );
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
		Convert<size_t>::Type GetTextLength( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getTextLength( ) );
		}



   	public:
		Convert<Ogre::UTFString>::Type GetSelectedText( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<Ogre::UTFString>::To( static_cast<ThisType*>(mNative)->getSelectedText( ) );
		}



   	public:
		Convert<bool>::Type IsTextSelect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->isTextSelect( ) );
		}



   	public:
		void SetTextSelect( Convert<size_t>::Type _start, Convert<size_t>::Type _end )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextSelect( Convert<size_t>::From(_start), Convert<size_t>::From(_end) );
		}



   	public:
		property Convert<const Ogre::UTFString &>::Type OnlyText
		{
			Convert<const Ogre::UTFString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const Ogre::UTFString &>::To( static_cast<ThisType*>(mNative)->getOnlyText() );
			}
			void set(Convert<const Ogre::UTFString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setOnlyText( Convert<const Ogre::UTFString &>::From(_value) );
			}
		}



	};

} // namespace MMyGUI
