
#include "ExportDefine.h"
#include <MyGUI.h>

namespace Export
{
	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_GetCaption(LPWStr & _caption, MyGUI::Widget* _widget)
		{
			_caption = _widget->getCaption().asWStr_c_str();
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetCaption(MyGUI::Widget* _widget, LPWStr _caption)
		{
			_widget->setCaption(_caption);
		}
	}

	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_GetNeedToolTip(bool & _value, MyGUI::Widget* _widget)
		{
			_value = _widget->getNeedToolTip();
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

}