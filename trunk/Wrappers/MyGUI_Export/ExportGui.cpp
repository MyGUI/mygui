#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "Generate/MyGUI_Export_MarshalingWidget.h"
#include "ExportMarshalingType.h"
#include <MyGUI.h>
#include "MyGUI_LogManager.h"

namespace Export
{
	//struct HasManagedParentTrait { };

	MYGUIEXPORT MyGUI::Widget* MYGUICALL ExportGui_CreateWidget(
		MyGUI::Widget* _parent,
		Convert<MyGUI::WidgetStyle>::Type _style,
		Convert<const std::string&>::Type _type,
		Convert<const std::string&>::Type _skin,
		Convert<MyGUI::IntCoord>::Type _coord,
		Convert<MyGUI::Align>::Type _align,
		Convert<const std::string&>::Type _layer,
		Convert<const std::string&>::Type _name
	)
	{
		MyGUI::WidgetPtr widget = nullptr;
		if (_parent == nullptr)
		{
			widget = MyGUI::Gui::getInstance().createWidgetT(
				Convert<const std::string&>::From(_type),
				Convert<const std::string&>::From(_skin),
				Convert<const MyGUI::IntCoord&>::From(_coord),
				Convert<MyGUI::Align>::From(_align),
				Convert<const std::string&>::From(_layer),
				Convert<const std::string&>::From(_name));
		}
		else
		{
			widget = _parent->createWidgetT(
				Convert<MyGUI::WidgetStyle>::From(_style),
				Convert<const std::string&>::From(_type),
				Convert<const std::string&>::From(_skin),
				Convert<const MyGUI::IntCoord&>::From(_coord),
				Convert<MyGUI::Align>::From(_align),
				Convert<const std::string&>::From(_layer),
				Convert<const std::string&>::From(_name));
		}

		return widget;
	}

	MYGUIEXPORT void MYGUICALL ExportGui_ExportTest()
	{
		MyGUI::xml::Document* doc = new MyGUI::xml::Document();
		doc->createDeclaration();
		doc->save(MyGUI::UString("MyGUI_ExportTest.xml"));
		delete doc;
	}

	MYGUIEXPORT void MYGUICALL ExportGui_DestroyWidget( MyGUI::Widget* _widget )
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(_widget);
	}

	MYGUIEXPORT void MYGUICALL ExportGui_WrapWidget(MyGUI::Widget* _widget )
	{
		//_widget->setUserData(_wrapper);
		_widget->setUserString("Internal__HasManagedParent", "");
	}

	MYGUIEXPORT void MYGUICALL ExportGui_UnwrapWidget( MyGUI::Widget* _widget )
	{
		//_widget->setUserData(MyGUI::Any::Null);
		_widget->clearUserString("Internal__HasManagedParent");
	}

	MYGUIEXPORT MyGUI::Widget* MYGUICALL ExportGui_GetManagedParent( MyGUI::Widget* _widget )
	{
		MyGUI::Widget* parent = _widget->getParent();
		while (parent != nullptr)
		{
			if (parent->isUserString("Internal__HasManagedParent"))
				return parent;
			parent = parent->getParent();
		}

		return nullptr;
	}

	MYGUIEXPORT Convert<const std::string&>::Type MYGUICALL ExportGui_GetWidgetType( MyGUI::Widget* _widget )
	{
		return Convert<const std::string&>::To(_widget->getTypeName());
	}

	MYGUIEXPORT void MYGUICALL ExportGui_LoadResource(
		Convert<const std::string&>::Type _source)
	{
		MyGUI::ResourceManager::getInstance().load(
			Convert<const std::string&>::From(_source) );
	}

	MYGUIEXPORT void MYGUICALL ExportGui_SetProperty(
		MyGUI::Widget* _widget,
		Convert<const std::string&>::Type _key ,
		Convert<const MyGUI::UString &>::Type _value )
	{
		const MyGUI::UString & value = Convert<const MyGUI::UString &>::From(_value);
		_widget->setProperty(
			Convert<const std::string&>::From(_key) ,
			value.asUTF8());
	}

	MYGUIEXPORT Convert<const std::string&>::Type MYGUICALL ExportGui_GetPath(
		Convert<const std::string&>::Type _filename)
	{
		return Convert<const std::string&>::To(
			MyGUI::DataManager::getInstance().getDataPath(
				Convert<const std::string&>::From(_filename) )
				);
	}

	MYGUIEXPORT void MYGUICALL ExportGui_Log(
		Convert<const std::string&>::Type _section,
		Convert<MyGUI::LogLevel>::Type _level,
		Convert<const std::string&>::Type _message )
	{
		MyGUI::LogManager::getInstance().log(
			Convert<const std::string&>::From(_section),
			Convert<MyGUI::LogLevel>::From(_level),
			Convert<const std::string&>::From(_message),
			__FILE__,
			__LINE__);
	}
}
