/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef EXPORT_WIDGET_ScrollView_H_
#define EXPORT_WIDGET_ScrollView_H_

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI_Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{
	//InsertPoint

	namespace ScopeScrollViewMethod_SetCanvasSize
	{
		MYGUIEXPORT void MYGUICALL ExportScrollView_SetCanvasSize__width__height(MyGUI::Widget* _native,
			Convert<int>::Type _width,
			Convert<int>::Type _height)
		{
			static_cast<MyGUI::ScrollView*>(_native)->setCanvasSize(
				Convert<int>::From(_width),
				Convert<int>::From(_height));
		}
	}
	namespace ScopeScrollViewProperty_ViewOffset
	{
		MYGUIEXPORT Convert<MyGUI::types::TPoint < int >>::Type MYGUICALL ExportScrollView_GetViewOffset(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::types::TPoint < int >>::To(static_cast<MyGUI::ScrollView*>(_native)->getViewOffset());
		}
		MYGUIEXPORT void MYGUICALL ExportScrollView_SetViewOffset(MyGUI::Widget* _native, Convert<const MyGUI::types::TPoint < int > &>::Type _value)
		{
			static_cast<MyGUI::ScrollView*>(_native)->setViewOffset(Convert<const MyGUI::types::TPoint < int > &>::From(_value));
		}
	}
	namespace ScopeScrollViewProperty_ViewCoord
	{
		MYGUIEXPORT Convert<MyGUI::types::TCoord < int >>::Type MYGUICALL ExportScrollView_GetViewCoord(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::types::TCoord < int >>::To(static_cast<MyGUI::ScrollView*>(_native)->getViewCoord());
		}
	}
	namespace ScopeScrollViewProperty_CanvasSize
	{
		MYGUIEXPORT Convert<MyGUI::types::TSize < int >>::Type MYGUICALL ExportScrollView_GetCanvasSize(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::types::TSize < int >>::To(static_cast<MyGUI::ScrollView*>(_native)->getCanvasSize());
		}
		MYGUIEXPORT void MYGUICALL ExportScrollView_SetCanvasSize(MyGUI::Widget* _native, Convert<const MyGUI::types::TSize < int > &>::Type _value)
		{
			static_cast<MyGUI::ScrollView*>(_native)->setCanvasSize(Convert<const MyGUI::types::TSize < int > &>::From(_value));
		}
	}
	namespace ScopeScrollViewProperty_CanvasAlign
	{
		MYGUIEXPORT Convert<MyGUI::Align>::Type MYGUICALL ExportScrollView_GetCanvasAlign(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::Align>::To(static_cast<MyGUI::ScrollView*>(_native)->getCanvasAlign());
		}
		MYGUIEXPORT void MYGUICALL ExportScrollView_SetCanvasAlign(MyGUI::Widget* _native, Convert<MyGUI::Align>::Type _value)
		{
			static_cast<MyGUI::ScrollView*>(_native)->setCanvasAlign(Convert<MyGUI::Align>::From(_value));
		}
	}
	namespace ScopeScrollViewProperty_VisibleHScroll
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportScrollView_IsVisibleHScroll(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::ScrollView*>(_native)->isVisibleHScroll());
		}
		MYGUIEXPORT void MYGUICALL ExportScrollView_SetVisibleHScroll(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::ScrollView*>(_native)->setVisibleHScroll(Convert<bool>::From(_value));
		}
	}
	
	namespace ScopeScrollViewProperty_VisibleVScroll
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportScrollView_IsVisibleVScroll(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::ScrollView*>(_native)->isVisibleVScroll());
		}
		MYGUIEXPORT void MYGUICALL ExportScrollView_SetVisibleVScroll(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::ScrollView*>(_native)->setVisibleVScroll(Convert<bool>::From(_value));
		}
	}
	
}

#endif
