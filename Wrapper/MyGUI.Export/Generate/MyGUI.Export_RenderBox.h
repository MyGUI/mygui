/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_RenderBox_H__
#define __EXPORT_WIDGET_RenderBox_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeRenderBoxEvent_UpdateViewport
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::RenderBox  *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::RenderBox  * _sender )
		{
			mExportHandle(
				Convert<MyGUI::RenderBox  *>::To( _sender ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportRenderBoxEvent_DelegateUpdateViewport( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportRenderBoxEvent_AdviseUpdateViewport( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::RenderBox* >(_widget)->eventUpdateViewport = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeRenderBoxProperty_BackgroundColour
	{
		MYGUIEXPORT Convert<const Ogre::ColourValue &>::Type MYGUICALL ExportRenderBox_GetBackgroundColour( MyGUI::Widget* _native )
		{
			return Convert<const Ogre::ColourValue &>::To( static_cast< MyGUI::RenderBox * >(_native)->getBackgroundColour( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportRenderBox_SetBackgroundColour( MyGUI::Widget* _native , Convert<const Ogre::ColourValue &>::Type _value )
		{
			static_cast< MyGUI::RenderBox * >(_native)->setBackgroundColour( Convert<const Ogre::ColourValue &>::From( _value ) );
		}
	}



   


   


   


   


   


   


   


   


   


   


}

#endif // __EXPORT_WIDGET_RenderBox_H__
