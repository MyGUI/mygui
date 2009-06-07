/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeEditBoxEvent_EditTextChange
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Edit *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Edit * _sender )
		{
			mExportHandle(
				Convert<MyGUI::Edit *>::To( _sender ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportEditBoxEvent_DelegateEditTextChange( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportEditBoxEvent_AdviseEditTextChange( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Edit* >(_widget)->eventEditTextChange = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeEditBoxEvent_EditSelectAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Edit *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Edit * _sender )
		{
			mExportHandle(
				Convert<MyGUI::Edit *>::To( _sender ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportEditBoxEvent_DelegateEditSelectAccept( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportEditBoxEvent_AdviseEditSelectAccept( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Edit* >(_widget)->eventEditSelectAccept = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   


   


   


   


   	namespace ScopeEditBoxProperty_VisibleHScroll
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_IsVisibleHScroll( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->isVisibleHScroll( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetVisibleHScroll( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setVisibleHScroll( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeEditBoxProperty_VisibleVScroll
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_IsVisibleVScroll( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->isVisibleVScroll( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetVisibleVScroll( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setVisibleVScroll( Convert<bool>::From( _value ) );
		}
	}
	


   


   


   


   


   


   


   	namespace ScopeEditBoxProperty_TabPrinting
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetTabPrinting( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getTabPrinting( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetTabPrinting( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setTabPrinting( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeEditBoxProperty_EditWordWrap
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetEditWordWrap( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getEditWordWrap( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetEditWordWrap( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setEditWordWrap( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeEditBoxMethod_SetPasswordChar
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetPasswordChar_char( MyGUI::Widget* _native,
			Convert<const Ogre::UTFString &>::Type _char )
		{
			static_cast< MyGUI::Edit * >(_native)->setPasswordChar(
				Convert<const Ogre::UTFString &>::From( _char ) );
		}
	}



   	namespace ScopeEditBoxProperty_PasswordChar
	{
		MYGUIEXPORT Convert<unsigned int>::Type MYGUICALL ExportEditBox_GetPasswordChar( MyGUI::Widget* _native )
		{
			return Convert<unsigned int>::To( static_cast< MyGUI::Edit * >(_native)->getPasswordChar( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetPasswordChar( MyGUI::Widget* _native , Convert<unsigned int>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setPasswordChar( Convert<unsigned int>::From( _value ) );
		}
	}



   	namespace ScopeEditBoxProperty_EditStatic
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetEditStatic( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getEditStatic( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetEditStatic( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setEditStatic( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeEditBoxProperty_EditMultiLine
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetEditMultiLine( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getEditMultiLine( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetEditMultiLine( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setEditMultiLine( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeEditBoxProperty_EditPassword
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetEditPassword( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getEditPassword( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetEditPassword( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setEditPassword( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeEditBoxProperty_EditReadOnly
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetEditReadOnly( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getEditReadOnly( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetEditReadOnly( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setEditReadOnly( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeEditBoxMethod_EraseText
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_EraseText_start_count( MyGUI::Widget* _native,
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _count )
		{
			static_cast< MyGUI::Edit * >(_native)->eraseText(
				Convert<size_t>::From( _start ) ,
				Convert<size_t>::From( _count ) );
		}
	}



   	namespace ScopeEditBoxMethod_AddText
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_AddText_text( MyGUI::Widget* _native,
			Convert<const Ogre::UTFString &>::Type _text )
		{
			static_cast< MyGUI::Edit * >(_native)->addText(
				Convert<const Ogre::UTFString &>::From( _text ) );
		}
	}



   	namespace ScopeEditBoxMethod_InsertText
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_InsertText_text_index( MyGUI::Widget* _native,
			Convert<const Ogre::UTFString &>::Type _text ,
			Convert<size_t>::Type _index )
		{
			static_cast< MyGUI::Edit * >(_native)->insertText(
				Convert<const Ogre::UTFString &>::From( _text ) ,
				Convert<size_t>::From( _index ) );
		}
	}



   	namespace ScopeEditBoxProperty_MaxTextLength
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetMaxTextLength( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getMaxTextLength( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetMaxTextLength( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setMaxTextLength( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeEditBoxProperty_OverflowToTheLeft
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetOverflowToTheLeft( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->getOverflowToTheLeft( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetOverflowToTheLeft( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setOverflowToTheLeft( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeEditBoxProperty_TextLength
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetTextLength( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getTextLength( ) );
		}
	}



   	namespace ScopeEditBoxProperty_OnlyText
	{
		MYGUIEXPORT Convert<Ogre::UTFString>::Type MYGUICALL ExportEditBox_GetOnlyText( MyGUI::Widget* _native )
		{
			return Convert<Ogre::UTFString>::To( static_cast< MyGUI::Edit * >(_native)->getOnlyText( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetOnlyText( MyGUI::Widget* _native , Convert<const Ogre::UTFString &>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setOnlyText( Convert<const Ogre::UTFString &>::From( _value ) );
		}
	}



   


   	namespace ScopeEditBoxProperty_TextCursor
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetTextCursor( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getTextCursor( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetTextCursor( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::Edit * >(_native)->setTextCursor( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeEditBoxMethod_SetTextSelectionColour
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetTextSelectionColour_colour( MyGUI::Widget* _native,
			Convert<const MyGUI::Colour &>::Type _colour )
		{
			static_cast< MyGUI::Edit * >(_native)->setTextSelectionColour(
				Convert<const MyGUI::Colour &>::From( _colour ) );
		}
	}



   	namespace ScopeEditBoxMethod_IsTextSelection
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_IsTextSelection( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Edit * >(_native)->isTextSelection( ) );
		}
	}



   	namespace ScopeEditBoxMethod_GetTextSelection
	{
		MYGUIEXPORT Convert<Ogre::UTFString>::Type MYGUICALL ExportEditBox_GetTextSelection( MyGUI::Widget* _native )
		{
			return Convert<Ogre::UTFString>::To( static_cast< MyGUI::Edit * >(_native)->getTextSelection( ) );
		}
	}



   	namespace ScopeEditBoxMethod_DeleteTextSelection
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_DeleteTextSelection( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Edit * >(_native)->deleteTextSelection( );
		}
	}



   	namespace ScopeEditBoxMethod_SetTextSelection
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetTextSelection_start_end( MyGUI::Widget* _native,
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _end )
		{
			static_cast< MyGUI::Edit * >(_native)->setTextSelection(
				Convert<size_t>::From( _start ) ,
				Convert<size_t>::From( _end ) );
		}
	}



   	namespace ScopeEditBoxMethod_GetTextInterval
	{
		MYGUIEXPORT Convert<Ogre::UTFString>::Type MYGUICALL ExportEditBox_GetTextInterval_start_count( MyGUI::Widget* _native,
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _count )
		{
			return Convert<Ogre::UTFString>::To( static_cast< MyGUI::Edit * >(_native)->getTextInterval(
				Convert<size_t>::From( _start ) ,
				Convert<size_t>::From( _count ) ));
		}
	}



   	namespace ScopeEditBoxMethod_GetTextSelectionLength
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetTextSelectionLength( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getTextSelectionLength( ) );
		}
	}



   	namespace ScopeEditBoxMethod_GetTextSelectionEnd
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetTextSelectionEnd( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getTextSelectionEnd( ) );
		}
	}



   	namespace ScopeEditBoxMethod_GetTextSelectionStart
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetTextSelectionStart( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Edit * >(_native)->getTextSelectionStart( ) );
		}
	}



   	namespace ScopeEditBoxMethod_SetTextIntervalColour
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetTextIntervalColour_start_count_colour( MyGUI::Widget* _native,
			Convert<size_t>::Type _start ,
			Convert<size_t>::Type _count ,
			Convert<const MyGUI::Colour &>::Type _colour )
		{
			static_cast< MyGUI::Edit * >(_native)->setTextIntervalColour(
				Convert<size_t>::From( _start ) ,
				Convert<size_t>::From( _count ) ,
				Convert<const MyGUI::Colour &>::From( _colour ) );
		}
	}



   


   


}
