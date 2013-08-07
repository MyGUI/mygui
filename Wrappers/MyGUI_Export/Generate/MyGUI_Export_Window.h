/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_Window_H__
#define __EXPORT_WIDGET_Window_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI_Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{
	//InsertPoint

	namespace ScopeWindowEvent_WindowChangeCoord
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::Window *);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Window * _sender)
		{
			mExportHandle(
				_sender);
		}
		
		MYGUIEXPORT void MYGUICALL ExportWindowEvent_DelegateWindowChangeCoord(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWindowEvent_AdviseWindowChangeCoord(MyGUI::Widget* _widget, bool _advise)
		{
			if (_advise)
				static_cast<MyGUI::Window*>(_widget)->eventWindowChangeCoord += MyGUI::newDelegate(OnEvent);
			else
				static_cast<MyGUI::Window*>(_widget)->eventWindowChangeCoord -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeWindowEvent_WindowButtonPressed
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::Window *,
			Convert<const std::string &>::Type);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Window * _sender,
			const std::string & _name)
		{
			mExportHandle(
				_sender,
				Convert<const std::string &>::To(_name));
		}
		
		MYGUIEXPORT void MYGUICALL ExportWindowEvent_DelegateWindowButtonPressed(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWindowEvent_AdviseWindowButtonPressed(MyGUI::Widget* _widget, bool _advise)
		{
			if (_advise)
				static_cast<MyGUI::Window*>(_widget)->eventWindowButtonPressed += MyGUI::newDelegate(OnEvent);
			else
				static_cast<MyGUI::Window*>(_widget)->eventWindowButtonPressed -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeWindowMethod_SetMaxSize
	{
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMaxSize__width__height(MyGUI::Widget* _native,
			Convert<int>::Type _width,
			Convert<int>::Type _height)
		{
			static_cast<MyGUI::Window*>(_native)->setMaxSize(
				Convert<int>::From(_width),
				Convert<int>::From(_height));
		}
	}
	namespace ScopeWindowMethod_SetMinSize
	{
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMinSize__width__height(MyGUI::Widget* _native,
			Convert<int>::Type _width,
			Convert<int>::Type _height)
		{
			static_cast<MyGUI::Window*>(_native)->setMinSize(
				Convert<int>::From(_width),
				Convert<int>::From(_height));
		}
	}
	namespace ScopeWindowMethod_SetVisibleSmooth
	{
		MYGUIEXPORT void MYGUICALL ExportWindow_SetVisibleSmooth__value(MyGUI::Widget* _native,
			Convert<bool>::Type _value)
		{
			static_cast<MyGUI::Window*>(_native)->setVisibleSmooth(
				Convert<bool>::From(_value));
		}
	}
	namespace ScopeWindowProperty_Movable
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWindow_GetMovable(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::Window*>(_native)->getMovable());
		}
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMovable(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::Window*>(_native)->setMovable(Convert<bool>::From(_value));
		}
	}
	namespace ScopeWindowProperty_ActionScale
	{
		MYGUIEXPORT Convert<const MyGUI::types::TCoord < int > &>::Type MYGUICALL ExportWindow_GetActionScale(MyGUI::Widget* _native)
		{
			return Convert<const MyGUI::types::TCoord < int > &>::To(static_cast<MyGUI::Window*>(_native)->getActionScale());
		}
	}
	namespace ScopeWindowProperty_Snap
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWindow_GetSnap(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::Window*>(_native)->getSnap());
		}
		MYGUIEXPORT void MYGUICALL ExportWindow_SetSnap(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::Window*>(_native)->setSnap(Convert<bool>::From(_value));
		}
	}
	namespace ScopeWindowProperty_MaxSize
	{
		MYGUIEXPORT Convert<MyGUI::types::TSize < int >>::Type MYGUICALL ExportWindow_GetMaxSize(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::types::TSize < int >>::To(static_cast<MyGUI::Window*>(_native)->getMaxSize());
		}
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMaxSize(MyGUI::Widget* _native, Convert<const MyGUI::types::TSize < int > &>::Type _value)
		{
			static_cast<MyGUI::Window*>(_native)->setMaxSize(Convert<const MyGUI::types::TSize < int > &>::From(_value));
		}
	}
	namespace ScopeWindowProperty_MinSize
	{
		MYGUIEXPORT Convert<MyGUI::types::TSize < int >>::Type MYGUICALL ExportWindow_GetMinSize(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::types::TSize < int >>::To(static_cast<MyGUI::Window*>(_native)->getMinSize());
		}
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMinSize(MyGUI::Widget* _native, Convert<const MyGUI::types::TSize < int > &>::Type _value)
		{
			static_cast<MyGUI::Window*>(_native)->setMinSize(Convert<const MyGUI::types::TSize < int > &>::From(_value));
		}
	}
	namespace ScopeWindowProperty_CaptionWidget
	{
		MYGUIEXPORT Convert<MyGUI::TextBox *>::Type MYGUICALL ExportWindow_GetCaptionWidget(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::TextBox *>::To(static_cast<MyGUI::Window*>(_native)->getCaptionWidget());
		}
	}
	namespace ScopeWindowProperty_AutoAlpha
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWindow_GetAutoAlpha(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::Window*>(_native)->getAutoAlpha());
		}
		MYGUIEXPORT void MYGUICALL ExportWindow_SetAutoAlpha(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::Window*>(_native)->setAutoAlpha(Convert<bool>::From(_value));
		}
	}
}

#endif
