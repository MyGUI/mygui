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
		property Convert<bool>::Type VisibleHScroll
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
		Convert<size_t>::Type GetVScrollRange( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getVScrollRange( ) );
		}



   	public:
		property Convert<bool>::Type VisibleVScroll
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
		void SetPasswordChar(
			Convert<const MyGUI::UString &>::Type _char )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPasswordChar(
				Convert<const MyGUI::UString &>::From(_char) );
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
		void SetTextSelectionColour(
			Convert<const MyGUI::Colour &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextSelectionColour(
				Convert<const MyGUI::Colour &>::From(_value) );
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
		Convert<size_t>::Type GetTextSelectionStart( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To(
				static_cast<ThisType*>(mNative)->getTextSelectionStart( ) );
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
