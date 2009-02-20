/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeEditEvent_EditTextChange
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Edit* _sender )
		{
			mExportHandle( *_sender->getUserData<IUnknown>() );
		}
		
		MYGUIEXPORT void MYGUICALL ExportEditEvent_DelegateEditTextChange( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportEditEvent_AdviseEditTextChange( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Edit* >(_widget)->eventEditTextChange = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeEditEvent_EditSelectAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Edit* _sender )
		{
			mExportHandle( *_sender->getUserData<IUnknown>() );
		}
		
		MYGUIEXPORT void MYGUICALL ExportEditEvent_DelegateEditSelectAccept( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportEditEvent_AdviseEditSelectAccept( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Edit* >(_widget)->eventEditSelectAccept = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeEditProperty_VisibleHScroll
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEdit_IsVisibleHScroll( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->isVisibleHScroll( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetVisibleHScroll( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setVisibleHScroll( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeEditProperty_VisibleVScroll
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEdit_IsVisibleVScroll( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->isVisibleVScroll( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetVisibleVScroll( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setVisibleVScroll( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeEditProperty_TabPrinting
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEdit_GetTabPrinting( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getTabPrinting( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetTabPrinting( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setTabPrinting( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeEditProperty_EditWordWrap
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEdit_GetEditWordWrap( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getEditWordWrap( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetEditWordWrap( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setEditWordWrap( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeEditMethod_SetPasswordChar
	{
		MYGUIEXPORT void MYGUICALL ExportEdit_SetPasswordChar_char( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _char )
		{
			static_cast< MyGUI::Edit * >(_native)->setPasswordChar(
				Convert<const Ogre::UTFString &>::From(_char) );
		}
	}



   	namespace ScopeEditProperty_PasswordChar
	{
		MYGUIEXPORT Convert< unsigned int >::Type MYGUICALL ExportEdit_GetPasswordChar( MyGUI::Widget* _native )
		{
			return Convert< unsigned int >::To( static_cast< MyGUI::Edit * >(_native)->getPasswordChar( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetPasswordChar( MyGUI::Widget* _native , Convert< unsigned int >::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setPasswordChar( Convert< unsigned int >::From( _value ) );
		}
	}
	


   	namespace ScopeEditProperty_EditStatic
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEdit_GetEditStatic( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getEditStatic( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetEditStatic( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setEditStatic( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeEditProperty_EditMultiLine
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEdit_GetEditMultiLine( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getEditMultiLine( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetEditMultiLine( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setEditMultiLine( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeEditProperty_EditPassword
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEdit_GetEditPassword( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getEditPassword( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetEditPassword( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setEditPassword( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeEditProperty_EditReadOnly
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEdit_GetEditReadOnly( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getEditReadOnly( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetEditReadOnly( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setEditReadOnly( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeEditMethod_EraseText
	{
		MYGUIEXPORT void MYGUICALL ExportEdit_EraseText_start_count( MyGUI::Widget* _native ,
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _count )
		{
			static_cast< MyGUI::Edit * >(_native)->eraseText(
				Convert<size_t>::From(_start) ,
				Convert<size_t>::From(_count) );
		}
	}


   	namespace ScopeEditMethod_EraseText
	{
		MYGUIEXPORT void MYGUICALL ExportEdit_EraseText_start( MyGUI::Widget* _native ,
			Convert<size_t>::Type _start )
		{
			static_cast< MyGUI::Edit * >(_native)->eraseText(
				Convert<size_t>::From(_start) );
		}
	}



   	namespace ScopeEditMethod_AddText
	{
		MYGUIEXPORT void MYGUICALL ExportEdit_AddText_text( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _text )
		{
			static_cast< MyGUI::Edit * >(_native)->addText(
				Convert<const Ogre::UTFString &>::From(_text) );
		}
	}



   	namespace ScopeEditMethod_InsertText
	{
		MYGUIEXPORT void MYGUICALL ExportEdit_InsertText_text_index( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _text ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::Edit * >(_native)->insertText(
				Convert<const Ogre::UTFString &>::From(_text) ,
				Convert<size_t>::From(_index) );
		}
	}


   	namespace ScopeEditMethod_InsertText
	{
		MYGUIEXPORT void MYGUICALL ExportEdit_InsertText_text( MyGUI::Widget* _native ,
			Convert<const Ogre::UTFString &>::Type _text )
		{
			static_cast< MyGUI::Edit * >(_native)->insertText(
				Convert<const Ogre::UTFString &>::From(_text) );
		}
	}



   	namespace ScopeEditProperty_MaxTextLength
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEdit_GetMaxTextLength( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getMaxTextLength( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetMaxTextLength( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setMaxTextLength( Convert<size_t>::From( _value ) );
		}
	}
	


   	namespace ScopeEditProperty_OverflowToTheLeft
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEdit_GetOverflowToTheLeft( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getOverflowToTheLeft( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetOverflowToTheLeft( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setOverflowToTheLeft( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeEditProperty_TextLength
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEdit_GetTextLength( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getTextLength( ) );
		}
	}
	


   	namespace ScopeEditProperty_OnlyText
	{
		MYGUIEXPORT Convert<const Ogre::UTFString &>::Type MYGUICALL ExportEdit_GetOnlyText( MyGUI::Widget* _native )
		{
			return Convert<const Ogre::UTFString &>::To( static_cast< MyGUI::Edit * >(_native)->getOnlyText( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetOnlyText( MyGUI::Widget* _native , Convert<const Ogre::UTFString &>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setOnlyText( Convert<const Ogre::UTFString &>::From( _value ) );
		}
	}
	


   	namespace ScopeEditProperty_TextCursor
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEdit_GetTextCursor( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getTextCursor( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEdit_SetTextCursor( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setTextCursor( Convert<size_t>::From( _value ) );
		}
	}
	


   	namespace ScopeEditMethod_SetTextSelectionColour
	{
		MYGUIEXPORT void MYGUICALL ExportEdit_SetTextSelectionColour_colour( MyGUI::Widget* _native ,
			Convert<const MyGUI::Colour &>::Type _colour )
		{
			static_cast< MyGUI::Edit * >(_native)->setTextSelectionColour(
				Convert<const MyGUI::Colour &>::From(_colour) );
		}
	}



   	namespace ScopeEditMethod_IsTextSelection
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEdit_IsTextSelection( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->isTextSelection( ) );
		}
	}



   	namespace ScopeEditMethod_GetTextSelection
	{
		MYGUIEXPORT Convert<Ogre::UTFString>::Type MYGUICALL ExportEdit_GetTextSelection( MyGUI::Widget* _native )
		{
			return Convert<Ogre::UTFString>::To( static_cast< MyGUI::Edit * >(_native)->getTextSelection( ) );
		}
	}



   	namespace ScopeEditMethod_DeleteTextSelection
	{
		MYGUIEXPORT void MYGUICALL ExportEdit_DeleteTextSelection( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Edit * >(_native)->deleteTextSelection( );
		}
	}



   	namespace ScopeEditMethod_SetTextSelection
	{
		MYGUIEXPORT void MYGUICALL ExportEdit_SetTextSelection_start_end( MyGUI::Widget* _native ,
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _end )
		{
			static_cast< MyGUI::Edit * >(_native)->setTextSelection(
				Convert<size_t>::From(_start) ,
				Convert<size_t>::From(_end) );
		}
	}



   	namespace ScopeEditMethod_GetTextInterval
	{
		MYGUIEXPORT Convert<Ogre::UTFString>::Type MYGUICALL ExportEdit_GetTextInterval_start_count( MyGUI::Widget* _native ,
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _count )
		{
			return Convert<Ogre::UTFString>::To( static_cast< MyGUI::Edit * >(_native)->getTextInterval(
				Convert<size_t>::From(_start) ,
				Convert<size_t>::From(_count) ) );
		}
	}



   	namespace ScopeEditMethod_GetTextSelectionLength
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEdit_GetTextSelectionLength( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getTextSelectionLength( ) );
		}
	}



   	namespace ScopeEditMethod_GetTextSelectionEnd
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEdit_GetTextSelectionEnd( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getTextSelectionEnd( ) );
		}
	}



   	namespace ScopeEditMethod_GetTextSelectionStart
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEdit_GetTextSelectionStart( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getTextSelectionStart( ) );
		}
	}



   	namespace ScopeEditMethod_SetTextIntervalColour
	{
		MYGUIEXPORT void MYGUICALL ExportEdit_SetTextIntervalColour_start_count_colour( MyGUI::Widget* _native ,
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _count ,
			Convert<const MyGUI::Colour &>::Type _colour )
		{
			static_cast< MyGUI::Edit * >(_native)->setTextIntervalColour(
				Convert<size_t>::From(_start) ,
				Convert<size_t>::From(_count) ,
				Convert<const MyGUI::Colour &>::From(_colour) );
		}
	}



}
