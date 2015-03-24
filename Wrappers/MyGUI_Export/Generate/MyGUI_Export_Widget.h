/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef EXPORT_WIDGET_Widget_H_
#define EXPORT_WIDGET_Widget_H_

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI_Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{
	//InsertPoint

	namespace ScopeWidgetEvent_ChangeCoord
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::Widget *);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender)
		{
			mExportHandle(
				_sender);
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateChangeCoord(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseChangeCoord(MyGUI::Widget* _widget, bool _advise)
		{
			if (_advise)
				static_cast<MyGUI::Widget*>(_widget)->eventChangeCoord += MyGUI::newDelegate(OnEvent);
			else
				static_cast<MyGUI::Widget*>(_widget)->eventChangeCoord -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeWidgetEvent_ChangeProperty
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			MyGUI::Widget *,
			Convert<const std::string &>::Type,
			Convert<const std::string &>::Type);
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender,
			const std::string & _key,
			const std::string & _value)
		{
			mExportHandle(
				_sender,
				Convert<const std::string &>::To(_key),
				Convert<const std::string &>::To(_value));
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateChangeProperty(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseChangeProperty(MyGUI::Widget* _widget, bool _advise)
		{
			if (_advise)
				static_cast<MyGUI::Widget*>(_widget)->eventChangeProperty += MyGUI::newDelegate(OnEvent);
			else
				static_cast<MyGUI::Widget*>(_widget)->eventChangeProperty -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeWidgetMethod_SetWidgetStyle
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetWidgetStyle__style__layer(MyGUI::Widget* _native,
			Convert<MyGUI::WidgetStyle>::Type _style,
			Convert<const std::string &>::Type _layer)
		{
			static_cast<MyGUI::Widget*>(_native)->setWidgetStyle(
				Convert<MyGUI::WidgetStyle>::From(_style),
				Convert<const std::string &>::From(_layer));
		}
	}
	namespace ScopeWidgetMethod_ChangeWidgetSkin
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_ChangeWidgetSkin__skinName(MyGUI::Widget* _native,
			Convert<const std::string &>::Type _skinName)
		{
			static_cast<MyGUI::Widget*>(_native)->changeWidgetSkin(
				Convert<const std::string &>::From(_skinName));
		}
	}
	namespace ScopeWidgetMethod_AttachToWidget
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_AttachToWidget__parent__style__layer(MyGUI::Widget* _native,
			Convert<MyGUI::Widget *>::Type _parent,
			Convert<MyGUI::WidgetStyle>::Type _style,
			Convert<const std::string &>::Type _layer)
		{
			static_cast<MyGUI::Widget*>(_native)->attachToWidget(
				Convert<MyGUI::Widget *>::From(_parent),
				Convert<MyGUI::WidgetStyle>::From(_style),
				Convert<const std::string &>::From(_layer));
		}
	}
	namespace ScopeWidgetMethod_DetachFromWidget
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_DetachFromWidget__layer(MyGUI::Widget* _native,
			Convert<const std::string &>::Type _layer)
		{
			static_cast<MyGUI::Widget*>(_native)->detachFromWidget(
				Convert<const std::string &>::From(_layer));
		}
	}
	namespace ScopeWidgetMethod_SetEnabledSilent
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetEnabledSilent__value(MyGUI::Widget* _native,
			Convert<bool>::Type _value)
		{
			static_cast<MyGUI::Widget*>(_native)->setEnabledSilent(
				Convert<bool>::From(_value));
		}
	}
	namespace ScopeWidgetMethod_FindWidget
	{
		MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportWidget_FindWidget__name(MyGUI::Widget* _native,
			Convert<const std::string &>::Type _name)
		{
			return Convert<MyGUI::Widget *>::To(static_cast<MyGUI::Widget*>(_native)->findWidget(
				Convert<const std::string &>::From(_name)));
		}
	}
	namespace ScopeWidgetMethod_GetChildAt
	{
		MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportWidget_GetChildAt__index(MyGUI::Widget* _native,
			Convert<size_t>::Type _index)
		{
			return Convert<MyGUI::Widget *>::To(static_cast<MyGUI::Widget*>(_native)->getChildAt(
				Convert<size_t>::From(_index)));
		}
	}
	namespace ScopeWidgetMethod_SetColour
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetColour__value(MyGUI::Widget* _native,
			Convert<const MyGUI::Colour &>::Type _value)
		{
			static_cast<MyGUI::Widget*>(_native)->setColour(
				Convert<const MyGUI::Colour &>::From(_value));
		}
	}
	namespace ScopeWidgetMethod_SetRealCoord
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealCoord__left__top__width__height(MyGUI::Widget* _native,
			Convert<float>::Type _left,
			Convert<float>::Type _top,
			Convert<float>::Type _width,
			Convert<float>::Type _height)
		{
			static_cast<MyGUI::Widget*>(_native)->setRealCoord(
				Convert<float>::From(_left),
				Convert<float>::From(_top),
				Convert<float>::From(_width),
				Convert<float>::From(_height));
		}
	}
	namespace ScopeWidgetMethod_SetRealSize
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealSize__width__height(MyGUI::Widget* _native,
			Convert<float>::Type _width,
			Convert<float>::Type _height)
		{
			static_cast<MyGUI::Widget*>(_native)->setRealSize(
				Convert<float>::From(_width),
				Convert<float>::From(_height));
		}
	}
	namespace ScopeWidgetMethod_SetRealPosition
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealPosition__left__top(MyGUI::Widget* _native,
			Convert<float>::Type _left,
			Convert<float>::Type _top)
		{
			static_cast<MyGUI::Widget*>(_native)->setRealPosition(
				Convert<float>::From(_left),
				Convert<float>::From(_top));
		}
	}
	namespace ScopeWidgetMethod_SetRealCoord
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealCoord__value(MyGUI::Widget* _native,
			Convert<const MyGUI::types::TCoord < float > &>::Type _value)
		{
			static_cast<MyGUI::Widget*>(_native)->setRealCoord(
				Convert<const MyGUI::types::TCoord < float > &>::From(_value));
		}
	}
	namespace ScopeWidgetMethod_SetRealSize
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealSize__value(MyGUI::Widget* _native,
			Convert<const MyGUI::types::TSize < float > &>::Type _value)
		{
			static_cast<MyGUI::Widget*>(_native)->setRealSize(
				Convert<const MyGUI::types::TSize < float > &>::From(_value));
		}
	}
	namespace ScopeWidgetMethod_SetRealPosition
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealPosition__value(MyGUI::Widget* _native,
			Convert<const MyGUI::types::TPoint < float > &>::Type _value)
		{
			static_cast<MyGUI::Widget*>(_native)->setRealPosition(
				Convert<const MyGUI::types::TPoint < float > &>::From(_value));
		}
	}
	namespace ScopeWidgetMethod_SetCoord
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetCoord__left__top__width__height(MyGUI::Widget* _native,
			Convert<int>::Type _left,
			Convert<int>::Type _top,
			Convert<int>::Type _width,
			Convert<int>::Type _height)
		{
			static_cast<MyGUI::Widget*>(_native)->setCoord(
				Convert<int>::From(_left),
				Convert<int>::From(_top),
				Convert<int>::From(_width),
				Convert<int>::From(_height));
		}
	}
	namespace ScopeWidgetMethod_SetSize
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetSize__width__height(MyGUI::Widget* _native,
			Convert<int>::Type _width,
			Convert<int>::Type _height)
		{
			static_cast<MyGUI::Widget*>(_native)->setSize(
				Convert<int>::From(_width),
				Convert<int>::From(_height));
		}
	}
	namespace ScopeWidgetMethod_SetPosition
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetPosition__left__top(MyGUI::Widget* _native,
			Convert<int>::Type _left,
			Convert<int>::Type _top)
		{
			static_cast<MyGUI::Widget*>(_native)->setPosition(
				Convert<int>::From(_left),
				Convert<int>::From(_top));
		}
	}
	namespace ScopeWidgetProperty_WidgetStyle
	{
		MYGUIEXPORT Convert<MyGUI::WidgetStyle>::Type MYGUICALL ExportWidget_GetWidgetStyle(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::WidgetStyle>::To(static_cast<MyGUI::Widget*>(_native)->getWidgetStyle());
		}
	}
	namespace ScopeWidgetProperty_ClientWidget
	{
		MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportWidget_GetClientWidget(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::Widget *>::To(static_cast<MyGUI::Widget*>(_native)->getClientWidget());
		}
	}
	namespace ScopeWidgetProperty_ClientCoord
	{
		MYGUIEXPORT Convert<MyGUI::types::TCoord < int >>::Type MYGUICALL ExportWidget_GetClientCoord(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::types::TCoord < int >>::To(static_cast<MyGUI::Widget*>(_native)->getClientCoord());
		}
	}
	namespace ScopeWidgetProperty_InheritedEnabled
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetInheritedEnabled(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::Widget*>(_native)->getInheritedEnabled());
		}
	}
	namespace ScopeWidgetProperty_Enabled
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetEnabled(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::Widget*>(_native)->getEnabled());
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetEnabled(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::Widget*>(_native)->setEnabled(Convert<bool>::From(_value));
		}
	}
	namespace ScopeWidgetProperty_ChildCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportWidget_GetChildCount(MyGUI::Widget* _native)
		{
			return Convert<size_t>::To(static_cast<MyGUI::Widget*>(_native)->getChildCount());
		}
	}
	namespace ScopeWidgetProperty_ParentSize
	{
		MYGUIEXPORT Convert<MyGUI::types::TSize < int >>::Type MYGUICALL ExportWidget_GetParentSize(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::types::TSize < int >>::To(static_cast<MyGUI::Widget*>(_native)->getParentSize());
		}
	}
	namespace ScopeWidgetProperty_Parent
	{
		MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportWidget_GetParent(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::Widget *>::To(static_cast<MyGUI::Widget*>(_native)->getParent());
		}
	}
	namespace ScopeWidgetProperty_RootWidget
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsRootWidget(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::Widget*>(_native)->isRootWidget());
		}
	}
	
	namespace ScopeWidgetProperty_InheritsAlpha
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetInheritsAlpha(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::Widget*>(_native)->getInheritsAlpha());
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetInheritsAlpha(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::Widget*>(_native)->setInheritsAlpha(Convert<bool>::From(_value));
		}
	}
	namespace ScopeWidgetProperty_Alpha
	{
		MYGUIEXPORT Convert<float>::Type MYGUICALL ExportWidget_GetAlpha(MyGUI::Widget* _native)
		{
			return Convert<float>::To(static_cast<MyGUI::Widget*>(_native)->getAlpha());
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetAlpha(MyGUI::Widget* _native, Convert<float>::Type _value)
		{
			static_cast<MyGUI::Widget*>(_native)->setAlpha(Convert<float>::From(_value));
		}
	}
	namespace ScopeWidgetProperty_Align
	{
		MYGUIEXPORT Convert<MyGUI::Align>::Type MYGUICALL ExportWidget_GetAlign(MyGUI::Widget* _native)
		{
			return Convert<MyGUI::Align>::To(static_cast<MyGUI::Widget*>(_native)->getAlign());
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetAlign(MyGUI::Widget* _native, Convert<MyGUI::Align>::Type _value)
		{
			static_cast<MyGUI::Widget*>(_native)->setAlign(Convert<MyGUI::Align>::From(_value));
		}
	}
	namespace ScopeWidgetProperty_InheritedVisible
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetInheritedVisible(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::Widget*>(_native)->getInheritedVisible());
		}
	}
	namespace ScopeWidgetProperty_Visible
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetVisible(MyGUI::Widget* _native)
		{
			return Convert<bool>::To(static_cast<MyGUI::Widget*>(_native)->getVisible());
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetVisible(MyGUI::Widget* _native, Convert<bool>::Type _value)
		{
			static_cast<MyGUI::Widget*>(_native)->setVisible(Convert<bool>::From(_value));
		}
	}
	namespace ScopeWidgetProperty_Name
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportWidget_GetName(MyGUI::Widget* _native)
		{
			return Convert<const std::string &>::To(static_cast<MyGUI::Widget*>(_native)->getName());
		}
	}
}

#endif
