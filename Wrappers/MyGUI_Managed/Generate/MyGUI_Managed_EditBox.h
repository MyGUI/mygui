/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI_Managed_TextBox.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class EditBox : public TextBox
		{

		private:
			typedef MyGUI::EditBox ThisType;

		public:
			EditBox() : TextBox() { }

		internal:
			EditBox( MyGUI::EditBox* _native ) : TextBox(_native) { }
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
			Convert<MyGUI::EditBox *>::Type _sender );
		event HandleEditTextChange^ EventEditTextChange
		{
			void add(HandleEditTextChange^ _value)
			{
				bool empty = mDelegateEditTextChange == nullptr;

				mDelegateEditTextChange += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventEditTextChange +=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::EditBox * > *>(
								new Delegate1< HandleEditTextChange^ ,
								MyGUI::EditBox * >(mDelegateEditTextChange) );
			}
			void remove(HandleEditTextChange^ _value)
			{
				mDelegateEditTextChange -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateEditTextChange == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventEditTextChange -=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::EditBox * > *>(
								new Delegate1< HandleEditTextChange^ ,
									MyGUI::EditBox * >(mDelegateEditTextChange) );
			}
		}
	private:
		HandleEditTextChange^ mDelegateEditTextChange;
	public:
		delegate void HandleEditSelectAccept(
			Convert<MyGUI::EditBox *>::Type _sender );
		event HandleEditSelectAccept^ EventEditSelectAccept
		{
			void add(HandleEditSelectAccept^ _value)
			{
				bool empty = mDelegateEditSelectAccept == nullptr;

				mDelegateEditSelectAccept += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventEditSelectAccept +=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::EditBox * > *>(
								new Delegate1< HandleEditSelectAccept^ ,
								MyGUI::EditBox * >(mDelegateEditSelectAccept) );
			}
			void remove(HandleEditSelectAccept^ _value)
			{
				mDelegateEditSelectAccept -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateEditSelectAccept == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventEditSelectAccept -=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::EditBox * > *>(
								new Delegate1< HandleEditSelectAccept^ ,
									MyGUI::EditBox * >(mDelegateEditSelectAccept) );
			}
		}
	private:
		HandleEditSelectAccept^ mDelegateEditSelectAccept;
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
		void SetPasswordChar(
			Convert<const MyGUI::UString &>::Type _char )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPasswordChar(
				Convert<const MyGUI::UString &>::From(_char) );
		}
	public:
		void EraseText(
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _count )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->eraseText(
				Convert<size_t>::From(_start) ,
				Convert<size_t>::From(_count) );
		}
	public:
		void AddText(
			Convert<const MyGUI::UString &>::Type _text )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addText(
				Convert<const MyGUI::UString &>::From(_text) );
		}
	public:
		void InsertText(
			Convert<const MyGUI::UString &>::Type _text ,
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertText(
				Convert<const MyGUI::UString &>::From(_text) ,
				Convert<size_t>::From(_index) );
		}
	public:
		void SetTextSelectionColour(
			Convert<const MyGUI::Colour &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextSelectionColour(
				Convert<const MyGUI::Colour &>::From(_value) );
		}
	public:
		void DeleteTextSelection( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->deleteTextSelection( );
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
		property Convert<size_t>::Type HScrollRange
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getHScrollRange() );
			}
		}
	
	public:
		property Convert<bool>::Type IsVisibleHScroll
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isVisibleHScroll() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVisibleHScroll( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<size_t>::Type VScrollPosition
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getVScrollPosition() );
			}
			void set(Convert<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVScrollPosition( Convert<size_t>::From(_value) );
			}
		}
	
	public:
		property Convert<size_t>::Type VScrollRange
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getVScrollRange() );
			}
		}
	
	public:
		property Convert<bool>::Type IsVisibleVScroll
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isVisibleVScroll() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVisibleVScroll( Convert<bool>::From(_value) );
			}
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
		property Convert<size_t>::Type TextLength
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getTextLength() );
			}
		}
	
	public:
		property Convert<MyGUI::UString>::Type OnlyText
		{
			Convert<MyGUI::UString>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::UString>::To( static_cast<ThisType*>(mNative)->getOnlyText() );
			}
			void set(Convert<const MyGUI::UString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setOnlyText( Convert<const MyGUI::UString &>::From(_value) );
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
		property Convert<bool>::Type IsTextSelection
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isTextSelection() );
			}
		}
	
	public:
		property Convert<MyGUI::UString>::Type TextSelection
		{
			Convert<MyGUI::UString>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::UString>::To( static_cast<ThisType*>(mNative)->getTextSelection() );
			}
		}
	
	public:
		property Convert<size_t>::Type TextSelectionLength
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getTextSelectionLength() );
			}
		}
	
	public:
		property Convert<size_t>::Type TextSelectionEnd
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getTextSelectionEnd() );
			}
		}
	
	public:
		property Convert<size_t>::Type TextSelectionStart
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getTextSelectionStart() );
			}
		}
	
		};

	} // namespace Managed
} // namespace MyGUI
