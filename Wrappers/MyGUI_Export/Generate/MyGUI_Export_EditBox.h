/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef EXPORT_WIDGET_EditBox_H_
#define EXPORT_WIDGET_EditBox_H_

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI_Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{
	//InsertPoint

	namespace ScopeEditBoxEvent_EditTextChange
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::EditBox *);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::EditBox * _sender)
		{
			mExportHandle(
				_sender);
		}
		
		MYGUIEXPORT void MYGUICALL ExportEditBoxEvent_DelegateEditTextChange(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportEditBoxEvent_AdviseEditTextChange(MyGUI::Widget* _widget, bool _advise)
		{
			if (_advise)
				static_cast<MyGUI::EditBox*>(_widget)->eventEditTextChange += MyGUI::newDelegate(OnEvent);
			else
				static_cast<MyGUI::EditBox*>(_widget)->eventEditTextChange -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeEditBoxEvent_EditSelectAccept
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::EditBox *);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::EditBox * _sender)
		{
			mExportHandle(
				_sender);
		}
		
		MYGUIEXPORT void MYGUICALL ExportEditBoxEvent_DelegateEditSelectAccept(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportEditBoxEvent_AdviseEditSelectAccept(MyGUI::Widget* _widget, bool _advise)
		{
			if (_advise)
				static_cast<MyGUI::EditBox*>(_widget)->eventEditSelectAccept += MyGUI::newDelegate(OnEvent);
			else
				static_cast<MyGUI::EditBox*>(_widget)->eventEditSelectAccept -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeEditBoxMethod_SetPasswordChar
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetPasswordChar__char(MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _char)
		{
			static_cast<MyGUI::EditBox*>(_native)->setPasswordChar(
				Convert<const MyGUI::UString &>::From(_char));
		}
	}
	namespace ScopeEditBoxMethod_EraseText
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_EraseText__start__count(MyGUI::Widget* _native,
			Convert<size_t>::Type _start,
			Convert<size_t>::Type _count)
		{
			static_cast<MyGUI::EditBox*>(_native)->eraseText(
				Convert<size_t>::From(_start),
				Convert<size_t>::From(_count));
		}
	}
	namespace ScopeEditBoxMethod_AddText
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_AddText__text(MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _text)
		{
			static_cast<MyGUI::EditBox*>(_native)->addText(
				Convert<const MyGUI::UString &>::From(_text));
		}
	}
	namespace ScopeEditBoxMethod_InsertText
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_InsertText__text__index(MyGUI::Widget* _native,
			Convert<const MyGUI::UString &>::Type _text,
			Convert<size_t>::Type _index)
		{
			static_cast<MyGUI::EditBox*>(_native)->insertText(
				Convert<const MyGUI::UString &>::From(_text),
				Convert<size_t>::From(_index));
		}
	}
	namespace ScopeEditBoxMethod_SetTextSelectionColour
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetTextSelectionColour__value(MyGUI::Widget* _native,
			Convert<const MyGUI::Colour &>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setTextSelectionColour(
				Convert<const MyGUI::Colour &>::From(_value));
		}
	}
	namespace ScopeEditBoxMethod_DeleteTextSelection
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_DeleteTextSelection(MyGUI::Widget* _native)
		{
			static_cast<MyGUI::EditBox*>(_native)->deleteTextSelection();
		}
	}
	namespace ScopeEditBoxMethod_SetTextSelection
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetTextSelection__start__end(MyGUI::Widget* _native,
			Convert<size_t>::Type _start,
			Convert<size_t>::Type _end)
		{
			static_cast<MyGUI::EditBox*>(_native)->setTextSelection(
				Convert<size_t>::From(_start),
				Convert<size_t>::From(_end));
		}
	}
	namespace ScopeEditBoxMethod_GetTextInterval
	{
		MYGUIEXPORT Convert<MyGUI::UString>::Type MYGUICALL ExportEditBox_GetTextInterval__start__count(MyGUI::Widget* _native,
			Convert<size_t>::Type _start,
			Convert<size_t>::Type _count)
		{
			return Convert<MyGUI::UString>::To(static_cast<MyGUI::EditBox*>(_native)->getTextInterval(
				Convert<size_t>::From(_start),
				Convert<size_t>::From(_count)));
		}
	}
	namespace ScopeEditBoxMethod_SetTextIntervalColour
	{
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetTextIntervalColour__start__count__colour(MyGUI::Widget* _native,
			Convert<size_t>::Type _start,
			Convert<size_t>::Type _count,
			Convert<const MyGUI::Colour &>::Type _colour)
		{
			static_cast<MyGUI::EditBox*>(_native)->setTextIntervalColour(
				Convert<size_t>::From(_start),
				Convert<size_t>::From(_count),
				Convert<const MyGUI::Colour &>::From(_colour));
		}
	}
	namespace ScopeEditBoxProperty_HScrollPosition
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetHScrollPosition(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::EditBox*>(_native)->getHScrollPosition());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetHScrollPosition(MyGUI::Widget* _native, Convert<size_t>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setHScrollPosition(Convert<size_t>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_HScrollRange
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetHScrollRange(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::EditBox*>(_native)->getHScrollRange());
		}
	}
	namespace ScopeEditBoxProperty_VisibleHScroll
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_IsVisibleHScroll(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::EditBox*>(_native)->isVisibleHScroll());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetVisibleHScroll(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setVisibleHScroll(Convert<bool>::From(_value));
		}
	}
	
	namespace ScopeEditBoxProperty_VScrollPosition
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetVScrollPosition(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::EditBox*>(_native)->getVScrollPosition());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetVScrollPosition(MyGUI::Widget* _native, Convert<size_t>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setVScrollPosition(Convert<size_t>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_VScrollRange
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetVScrollRange(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::EditBox*>(_native)->getVScrollRange());
		}
	}
	namespace ScopeEditBoxProperty_VisibleVScroll
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_IsVisibleVScroll(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::EditBox*>(_native)->isVisibleVScroll());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetVisibleVScroll(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setVisibleVScroll(Convert<bool>::From(_value));
		}
	}
	
	namespace ScopeEditBoxProperty_InvertSelected
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetInvertSelected(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::EditBox*>(_native)->getInvertSelected());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetInvertSelected(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setInvertSelected(Convert<bool>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_TabPrinting
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetTabPrinting(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::EditBox*>(_native)->getTabPrinting());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetTabPrinting(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setTabPrinting(Convert<bool>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_EditWordWrap
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetEditWordWrap(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::EditBox*>(_native)->getEditWordWrap());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetEditWordWrap(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setEditWordWrap(Convert<bool>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_PasswordChar
	{
		MYGUIEXPORT Convert<unsigned int>::Type MYGUICALL ExportEditBox_GetPasswordChar(MyGUI::Widget* _native)
		{
			return Convert<unsigned int>::To(static_cast<MyGUI::EditBox*>(_native)->getPasswordChar());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetPasswordChar(MyGUI::Widget* _native, Convert<unsigned int>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setPasswordChar(Convert<unsigned int>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_EditStatic
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetEditStatic(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::EditBox*>(_native)->getEditStatic());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetEditStatic(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setEditStatic(Convert<bool>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_EditMultiLine
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetEditMultiLine(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::EditBox*>(_native)->getEditMultiLine());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetEditMultiLine(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setEditMultiLine(Convert<bool>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_EditPassword
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetEditPassword(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::EditBox*>(_native)->getEditPassword());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetEditPassword(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setEditPassword(Convert<bool>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_EditReadOnly
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetEditReadOnly(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::EditBox*>(_native)->getEditReadOnly());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetEditReadOnly(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setEditReadOnly(Convert<bool>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_MaxTextLength
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetMaxTextLength(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::EditBox*>(_native)->getMaxTextLength());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetMaxTextLength(MyGUI::Widget* _native, Convert<size_t>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setMaxTextLength(Convert<size_t>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_OverflowToTheLeft
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_GetOverflowToTheLeft(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::EditBox*>(_native)->getOverflowToTheLeft());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetOverflowToTheLeft(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setOverflowToTheLeft(Convert<bool>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_TextLength
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetTextLength(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::EditBox*>(_native)->getTextLength());
		}
	}
	namespace ScopeEditBoxProperty_OnlyText
	{
		MYGUIEXPORT Convert<MyGUI::UString>::Type MYGUICALL ExportEditBox_GetOnlyText(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::UString>::To(static_cast<MyGUI::EditBox*>(_native)->getOnlyText());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetOnlyText(MyGUI::Widget* _native, Convert<const MyGUI::UString &>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setOnlyText(Convert<const MyGUI::UString &>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_TextCursor
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetTextCursor(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::EditBox*>(_native)->getTextCursor());
		}
		MYGUIEXPORT void MYGUICALL ExportEditBox_SetTextCursor(MyGUI::Widget* _native, Convert<size_t>::Type _value)
		{
			static_cast<MyGUI::EditBox*>(_native)->setTextCursor(Convert<size_t>::From(_value));
		}
	}
	namespace ScopeEditBoxProperty_TextSelection
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportEditBox_IsTextSelection(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::EditBox*>(_native)->isTextSelection());
		}
	}
	
	namespace ScopeEditBoxProperty_TextSelection
	{
		MYGUIEXPORT Convert<MyGUI::UString>::Type MYGUICALL ExportEditBox_GetTextSelection(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::UString>::To(static_cast<MyGUI::EditBox*>(_native)->getTextSelection());
		}
	}
	namespace ScopeEditBoxProperty_TextSelectionLength
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetTextSelectionLength(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::EditBox*>(_native)->getTextSelectionLength());
		}
	}
	namespace ScopeEditBoxProperty_TextSelectionEnd
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetTextSelectionEnd(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::EditBox*>(_native)->getTextSelectionEnd());
		}
	}
	namespace ScopeEditBoxProperty_TextSelectionStart
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportEditBox_GetTextSelectionStart(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::EditBox*>(_native)->getTextSelectionStart());
		}
	}
}

#endif
