
#include "ExportDefine.h"
#include <MyGUI.h>

/*namespace Export
{
	namespace
	{
		MYGUIEXPORT LPWStr MYGUICALL ExportWidget_GetCaption(MyGUI::Widget* _widget)
		{
			return _widget->getCaption().asWStr_c_str();
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetCaption(MyGUI::Widget* _widget, LPWStr _caption)
		{
			_widget->setCaption(_caption);
		}
	}

	namespace
	{
		MYGUIEXPORT bool MYGUICALL ExportWidget_GetNeedToolTip(MyGUI::Widget* _widget)
		{
			return _widget->getNeedToolTip();
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetNeedToolTip(MyGUI::Widget* _widget, bool _value)
		{
			_widget->setNeedToolTip(_value);
		}
	}


	namespace
	{
		typedef void (MYGUICALLBACK *ExportHandleToolTip)(IUnknown _wrapper, const MyGUI::ToolTipInfo& _info);
		ExportHandleToolTip mExportHandleToolTip = nullptr;
		void OnToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
		{
			mExportHandleToolTip(*_sender->getUserData<IUnknown>(), _info);
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_ExportToolTip(ExportHandleToolTip _delegate)
		{
			mExportHandleToolTip = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseToolTip(MyGUI::Widget* _widget, bool _advise)
		{
			_widget->eventToolTip = _advise ? MyGUI::newDelegate(OnToolTip) : nullptr;
		}
	}

}*/