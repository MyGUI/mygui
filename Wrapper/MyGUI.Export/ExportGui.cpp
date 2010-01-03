
#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "Generate/MyGUI.Export_MarshalingWidget.h"
#include "ExportMarshalingType.h"
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
		typedef Interface (MYGUICALLBACK *ExportHandle)( Convert<const std::string &>::Type _type, Interface _parent, MyGUI::Widget* _widget );
		ExportHandle mExportHandle = nullptr;

		MYGUIEXPORT void MYGUICALL ExportGui_SetCreatorWrapps( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
	}

	namespace ScopeGuiEvent_GetNativeByWrapper
	{
		typedef MyGUI::Widget* (MYGUICALLBACK *ExportHandle)( Interface _wrapper );
		ExportHandle mExportHandle = nullptr;

		MYGUIEXPORT void MYGUICALL ExportGui_SetGetNativeByWrapper( ExportHandle _delegate )
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
		return ScopeGuiEvent_CreateWrapp::mExportHandle( Convert<const std::string &>::To( _widget->getTypeName() ), getMangedParent(_widget), _widget );
	}

	MyGUI::Widget* GetNativeByWrapper( Interface _wrapper )
	{
		if (_wrapper == nullptr) return nullptr;
		return ScopeGuiEvent_GetNativeByWrapper::mExportHandle( _wrapper );
	}


	MYGUIEXPORT bool MYGUICALL ExportGui_GetKeyFocus( )
	{
		return MyGUI::InputManager::getInstance().isFocusKey();
	}

	MYGUIEXPORT bool MYGUICALL ExportGui_GetMouseFocus( )
	{
		return MyGUI::InputManager::getInstance().isFocusMouse();
	}

	MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportGui_GetKeyFocusWidget( )
	{
		return Convert<MyGUI::Widget *>::To( MyGUI::InputManager::getInstance().getKeyFocusWidget() );
	}

	MYGUIEXPORT void MYGUICALL ExportGui_SetKeyFocusWidget(
		MyGUI::Widget * _widget )
	{
		MyGUI::InputManager::getInstance().setKeyFocusWidget(
			_widget );
	}

	MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportGui_GetMouseFocusWidget( )
	{
		return Convert<MyGUI::Widget *>::To( MyGUI::InputManager::getInstance().getMouseFocusWidget() );
	}

	MYGUIEXPORT void MYGUICALL ExportGui_ResetKeyFocusWidget( )
	{
		MyGUI::InputManager::getInstance().resetKeyFocusWidget( );
	}

	MYGUIEXPORT void MYGUICALL ExportGui_AddWidgetModal(
		MyGUI::Widget* _widget )
	{
		MyGUI::InputManager::getInstance().addWidgetModal(
			_widget );
	}

	MYGUIEXPORT void MYGUICALL ExportGui_RemoveWidgetModal(
		MyGUI::Widget* _widget )
	{
		MyGUI::InputManager::getInstance().removeWidgetModal(
			_widget );
	}

	MYGUIEXPORT void MYGUICALL ExportGui_AttachToLayer(
		Convert<const std::string &>::Type _layer ,
		MyGUI::Widget * _widget )
	{
		MyGUI::LayerManager::getInstance().attachToLayerNode(
			Convert<const std::string &>::From(_layer) ,
			_widget );
	}

	MYGUIEXPORT void MYGUICALL ExportGui_LoadResource(
		Convert<const std::string &>::Type _source)
	{
		MyGUI::ResourceManager::getInstance().load(
			Convert<const std::string &>::From(_source) );
	}

	MYGUIEXPORT void MYGUICALL ExportGui_UpWidget(
		MyGUI::Widget* _widget )
	{
		MyGUI::LayerManager::getInstance().upLayerItem(
			_widget );
	}

	MYGUIEXPORT void MYGUICALL ExportGui_SetProperty(
		MyGUI::Widget* _widget,
		Convert<const std::string &>::Type _key ,
		Convert<const std::string &>::Type _value )
	{
		_widget->setProperty(
			Convert<const std::string &>::From(_key) ,
			Convert<const std::string &>::From(_value) );
	}

	MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportGui_GetPath(
		Convert<const std::string &>::Type _filename)
	{
		return Convert<const std::string &>::To( 
		MyGUI::DataManager::getInstance().getDataPath(
			Convert<const std::string &>::From(_filename) )
			);
	}

}
