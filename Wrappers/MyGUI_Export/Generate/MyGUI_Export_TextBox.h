/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef EXPORT_WIDGET_TextBox_H_
#define EXPORT_WIDGET_TextBox_H_

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI_Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{
	//InsertPoint

	namespace ScopeTextBoxMethod_SetCaptionWithReplacing
	{
		MYGUIEXPORT void MYGUICALL ExportTextBox_SetCaptionWithReplacing__value(MyGUI::Widget* _native,
			Convert<const std::string &>::Type _value)
		{
			static_cast<MyGUI::TextBox*>(_native)->setCaptionWithReplacing(
				Convert<const std::string &>::From(_value));
		}
	}
	namespace ScopeTextBoxProperty_TextShadow
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportTextBox_GetTextShadow(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::TextBox*>(_native)->getTextShadow());
		}
		MYGUIEXPORT void MYGUICALL ExportTextBox_SetTextShadow(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::TextBox*>(_native)->setTextShadow(Convert<bool>::From(_value));
		}
	}
	namespace ScopeTextBoxProperty_TextShadowColour
	{
		MYGUIEXPORT Convert<const MyGUI::Colour &>::Type MYGUICALL ExportTextBox_GetTextShadowColour(MyGUI::Widget* _native)
		{
			return Convert<const MyGUI::Colour &>::To(static_cast<MyGUI::TextBox*>(_native)->getTextShadowColour());
		}
		MYGUIEXPORT void MYGUICALL ExportTextBox_SetTextShadowColour(MyGUI::Widget* _native, Convert<const MyGUI::Colour &>::Type _value)
		{
			static_cast<MyGUI::TextBox*>(_native)->setTextShadowColour(Convert<const MyGUI::Colour &>::From(_value));
		}
	}
	namespace ScopeTextBoxProperty_TextColour
	{
		MYGUIEXPORT Convert<const MyGUI::Colour &>::Type MYGUICALL ExportTextBox_GetTextColour(MyGUI::Widget* _native)
		{
			return Convert<const MyGUI::Colour &>::To(static_cast<MyGUI::TextBox*>(_native)->getTextColour());
		}
		MYGUIEXPORT void MYGUICALL ExportTextBox_SetTextColour(MyGUI::Widget* _native, Convert<const MyGUI::Colour &>::Type _value)
		{
			static_cast<MyGUI::TextBox*>(_native)->setTextColour(Convert<const MyGUI::Colour &>::From(_value));
		}
	}
	namespace ScopeTextBoxProperty_TextAlign
	{
		MYGUIEXPORT Convert<MyGUI::Align>::Type MYGUICALL ExportTextBox_GetTextAlign(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::Align>::To(static_cast<MyGUI::TextBox*>(_native)->getTextAlign());
		}
		MYGUIEXPORT void MYGUICALL ExportTextBox_SetTextAlign(MyGUI::Widget* _native, Convert<MyGUI::Align>::Type _value)
		{
			static_cast<MyGUI::TextBox*>(_native)->setTextAlign(Convert<MyGUI::Align>::From(_value));
		}
	}
	namespace ScopeTextBoxProperty_FontHeight
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportTextBox_GetFontHeight(MyGUI::Widget* _native)
		{
			return Convert<int>::To(static_cast<MyGUI::TextBox*>(_native)->getFontHeight());
		}
		MYGUIEXPORT void MYGUICALL ExportTextBox_SetFontHeight(MyGUI::Widget* _native, Convert<int>::Type _value)
		{
			static_cast<MyGUI::TextBox*>(_native)->setFontHeight(Convert<int>::From(_value));
		}
	}
	namespace ScopeTextBoxProperty_FontName
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportTextBox_GetFontName(MyGUI::Widget* _native)
		{
			return Convert<const std::string &>::To(static_cast<MyGUI::TextBox*>(_native)->getFontName());
		}
		MYGUIEXPORT void MYGUICALL ExportTextBox_SetFontName(MyGUI::Widget* _native, Convert<const std::string &>::Type _value)
		{
			static_cast<MyGUI::TextBox*>(_native)->setFontName(Convert<const std::string &>::From(_value));
		}
	}
	namespace ScopeTextBoxProperty_Caption
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportTextBox_GetCaption(MyGUI::Widget* _native)
		{
			return Convert<const MyGUI::UString &>::To(static_cast<MyGUI::TextBox*>(_native)->getCaption());
		}
		MYGUIEXPORT void MYGUICALL ExportTextBox_SetCaption(MyGUI::Widget* _native, Convert<const MyGUI::UString &>::Type _value)
		{
			static_cast<MyGUI::TextBox*>(_native)->setCaption(Convert<const MyGUI::UString &>::From(_value));
		}
	}
	namespace ScopeTextBoxProperty_TextSize
	{
		MYGUIEXPORT Convert<MyGUI::types::TSize < int >>::Type MYGUICALL ExportTextBox_GetTextSize(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::types::TSize < int >>::To(static_cast<MyGUI::TextBox*>(_native)->getTextSize());
		}
	}
	namespace ScopeTextBoxProperty_TextRegion
	{
		MYGUIEXPORT Convert<MyGUI::types::TCoord < int >>::Type MYGUICALL ExportTextBox_GetTextRegion(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::types::TCoord < int >>::To(static_cast<MyGUI::TextBox*>(_native)->getTextRegion());
		}
	}
}

#endif
