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
		delegate void HandleEditTextChange( ConvertToType<MyGUI::Widget *>::Type _value1 );
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
		delegate void HandleEditSelectAccept( ConvertToType<MyGUI::Widget *>::Type _value1 );
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
		void setFontHeight( ConvertToType< unsigned short >::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setFontHeight( ConvertToType< unsigned short >::ConvertFromValue(_height) );
		}



   	public:
		void setFontName( ConvertToType<const std::string &>::Type _font )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setFontName( ConvertToType<const std::string &>::ConvertFromValue(_font) );
		}



   	public:
		ConvertToType<bool>::Type isShowHScroll( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isShowHScroll( ) );
		}



   	public:
		void showHScroll( ConvertToType<bool>::Type _show )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showHScroll( ConvertToType<bool>::ConvertFromValue(_show) );
		}



   	public:
		ConvertToType<bool>::Type isShowVScroll( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isShowVScroll( ) );
		}



   	public:
		void showVScroll( ConvertToType<bool>::Type _show )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showVScroll( ConvertToType<bool>::ConvertFromValue(_show) );
		}



   	public:
		void setPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void setCoord( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setSize( ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top) );
		}



   	public:
		void setCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void setSize( ConvertToType<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_size) );
		}



   	public:
		void setPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::Type _point )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::ConvertFromValue(_point) );
		}



   	public:
		property ConvertToType<bool>::Type TabPrinting
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getTabPrinting() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTabPrinting( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<bool>::Type EditWordWrap
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getEditWordWrap() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEditWordWrap( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		void setPasswordChar( ConvertToType<const Ogre::UTFString &>::Type _char )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPasswordChar( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_char) );
		}



   	public:
		property ConvertToType< unsigned int >::Type PasswordChar
		{
			ConvertToType< unsigned int >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType< unsigned int >::ConvertToValue( static_cast<ThisType*>(mNative)->getPasswordChar() );
			}
			void set(ConvertToType< unsigned int >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPasswordChar( ConvertToType< unsigned int >::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<bool>::Type EditStatic
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getEditStatic() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEditStatic( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<bool>::Type EditMultiLine
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getEditMultiLine() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEditMultiLine( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<bool>::Type EditPassword
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getEditPassword() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEditPassword( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<bool>::Type EditReadOnly
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getEditReadOnly() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEditReadOnly( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		void setTextColour( ConvertToType<size_t>::Type _start, ConvertToType<size_t>::Type _count, ConvertToType<const MyGUI::Colour &>::Type _colour )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextColour( ConvertToType<size_t>::ConvertFromValue(_start), ConvertToType<size_t>::ConvertFromValue(_count), ConvertToType<const MyGUI::Colour &>::ConvertFromValue(_colour) );
		}



   	public:
		void setTextSelectColour( ConvertToType<const MyGUI::Colour &>::Type _colour )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextSelectColour( ConvertToType<const MyGUI::Colour &>::ConvertFromValue(_colour) );
		}



   	public:
		void eraseText( ConvertToType<size_t>::Type _start, ConvertToType<size_t>::Type _count )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->eraseText( ConvertToType<size_t>::ConvertFromValue(_start), ConvertToType<size_t>::ConvertFromValue(_count) );
		}



   	public:
		void addText( ConvertToType<const Ogre::UTFString &>::Type _text )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->addText( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_text) );
		}



   	public:
		void insertText( ConvertToType<const Ogre::UTFString &>::Type _text, ConvertToType<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->insertText( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_text), ConvertToType<size_t>::ConvertFromValue(_index) );
		}



   	public:
		ConvertToType<bool>::Type deleteTextSelect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->deleteTextSelect( ) );
		}



   	public:
		ConvertToType<Ogre::UTFString>::Type getText( ConvertToType<size_t>::Type _start, ConvertToType<size_t>::Type _count )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<Ogre::UTFString>::ConvertToValue( static_cast<ThisType*>(mNative)->getText( ConvertToType<size_t>::ConvertFromValue(_start), ConvertToType<size_t>::ConvertFromValue(_count) ) );
		}



   	public:
		property ConvertToType<size_t>::Type MaxTextLength
		{
			ConvertToType<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getMaxTextLength() );
			}
			void set(ConvertToType<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMaxTextLength( ConvertToType<size_t>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<bool>::Type OverflowToTheLeft
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getOverflowToTheLeft() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setOverflowToTheLeft( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		void setTextAlign( ConvertToType< MyGUI::Align >::Type _align )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextAlign( ConvertToType< MyGUI::Align >::ConvertFromValue(_align) );
		}



   	public:
		property ConvertToType<size_t>::Type TextCursor
		{
			ConvertToType<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getTextCursor() );
			}
			void set(ConvertToType<size_t>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTextCursor( ConvertToType<size_t>::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType<size_t>::Type getTextLength( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getTextLength( ) );
		}



   	public:
		ConvertToType<Ogre::UTFString>::Type getSelectedText( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<Ogre::UTFString>::ConvertToValue( static_cast<ThisType*>(mNative)->getSelectedText( ) );
		}



   	public:
		ConvertToType<bool>::Type isTextSelect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isTextSelect( ) );
		}



   	public:
		void setTextSelect( ConvertToType<size_t>::Type _start, ConvertToType<size_t>::Type _end )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setTextSelect( ConvertToType<size_t>::ConvertFromValue(_start), ConvertToType<size_t>::ConvertFromValue(_end) );
		}



   	public:
		void getTextSelect( ConvertToType<size_t &>::Type _start, ConvertToType<size_t &>::Type _end )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->getTextSelect( ConvertToType<size_t &>::ConvertFromValue(_start), ConvertToType<size_t &>::ConvertFromValue(_end) );
		}



   	public:
		ConvertToType<Ogre::UTFString>::Type getOnlyText( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<Ogre::UTFString>::ConvertToValue( static_cast<ThisType*>(mNative)->getOnlyText( ) );
		}



   	public:
		void setOnlyText( ConvertToType<const Ogre::UTFString &>::Type _text )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setOnlyText( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_text) );
		}



   	public:
		property ConvertToType<const Ogre::UTFString &>::Type Caption
		{
			ConvertToType<const Ogre::UTFString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getCaption() );
			}
			void set(ConvertToType<const Ogre::UTFString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCaption( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType<const std::string &>::Type getTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



   	public:
		ConvertToType<bool>::Type isType( ConvertToType<const std::type_info &>::Type t )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isType( ConvertToType<const std::type_info &>::ConvertFromValue(t) ) );
		}



	};

} // namespace MMyGUI
