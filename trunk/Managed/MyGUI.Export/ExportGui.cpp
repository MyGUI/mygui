
#include "ExportDefine.h"
#include <MyGUI.h>

namespace Export
{

	MYGUIEXPORT MyGUI::Widget* MYGUICALL ExportGui_CreateWidget(
		IUnknown _wrapper,
		MyGUI::Widget* _parent,
		MyGUI::WidgetStyle _style,
		LPStr _type,
		LPStr _skin,
		const MyGUI::IntCoord& _coord,
		MyGUI::Align _align,
		LPStr _layer,
		LPStr _name
		)
	{
		MyGUI::WidgetPtr widget = nullptr;
		if (_parent == nullptr)
		{
			widget = MyGUI::Gui::getInstance().createWidgetT(_type, _skin, _coord, _align, _layer, _name);
		}
		else
		{
			widget = _parent->createWidgetT(_style, _type, _skin, _coord, _align, _layer, _name);
		}

		widget->setUserData(_wrapper);

		return widget;
	}

	MYGUIEXPORT void MYGUICALL ExportGui_DestroyWidget(MyGUI::Widget* _widget)
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(_widget);
	}

	MYGUIEXPORT void MYGUICALL ExportGui_WrapWidget(IUnknown _wrapper, MyGUI::Widget* _widget)
	{
		_widget->setUserData(_wrapper);
	}

	MYGUIEXPORT void MYGUICALL ExportGui_UnwrapWidget(MyGUI::Widget* _widget)
	{
		_widget->setUserData(MyGUI::Any::Null);
	}

}