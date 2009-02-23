
#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include <MyGUI.h>

namespace Export
{

	MYGUIEXPORT MyGUI::Widget* MYGUICALL ExportGui_CreateWidget(
		Interface _wrapper,
		MyGUI::Widget* _parent,
		MyGUI::WidgetStyle _style,
		Convert<const std::string &>::Type _type,
		Convert<const std::string &>::Type _skin,
		const MyGUI::IntCoord& _coord,
		MyGUI::Align _align,
		Convert<const std::string &>::Type _layer,
		Convert<const std::string &>::Type _name
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

	MYGUIEXPORT void MYGUICALL ExportGui_DestroyWidget( MyGUI::Widget* _widget )
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(_widget);
	}

	MYGUIEXPORT void MYGUICALL ExportGui_WrapWidget( Interface _wrapper, MyGUI::Widget* _widget )
	{
		_widget->setUserData(_wrapper);
	}

	MYGUIEXPORT void MYGUICALL ExportGui_UnwrapWidget( MyGUI::Widget* _widget )
	{
		_widget->setUserData(MyGUI::Any::Null);
	}

	namespace ScopeGuiEvent_CreateWrapp
	{
		typedef Interface (MYGUICALLBACK *ExportHandle)( Interface _parent, Convert<const std::string &>::Type _type, MyGUI::Widget* _widget );
		ExportHandle mExportHandle = nullptr;

		MYGUIEXPORT void MYGUICALL ExportGui_SetCreatorWrapps( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
	}

	Interface getMangedParent( MyGUI::WidgetPtr _widget )
	{
		MyGUI::WidgetPtr parent = _widget->getParent();
		while (parent != nullptr)
		{
			Interface * obj = parent->getUserData<Interface>(false);
			if (obj != nullptr) return *obj;
			parent = parent->getParent();
		}
		return nullptr;
	}

	Interface CreateWrapper( MyGUI::WidgetPtr _widget )
	{
		if (_widget == nullptr) return nullptr;
		return ScopeGuiEvent_CreateWrapp::mExportHandle( getMangedParent(_widget), Convert<const std::string &>::To( _widget->getTypeName() ), _widget );
	}

}