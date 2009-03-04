/*!
	@file
	@author		Georgiy Evmenov
	@date		08/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_DELEGATE_MANAGER_H__
#define __MYGUI_DELEGATE_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Common.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	#define DELEGATE_MANAGER_EVENT0(name) void name(MyGUI::WidgetPtr _sender){callDelegate(_sender, _sender->getUserString(#name), #name);}
	#define DELEGATE_MANAGER_EVENT1(name, param1) void name(MyGUI::WidgetPtr _sender, param1){callDelegate(_sender, _sender->getUserString(#name), #name);}
	#define DELEGATE_MANAGER_EVENT2(name, param1, param2) void name(MyGUI::WidgetPtr _sender, param1, param2){callDelegate(_sender, _sender->getUserString(#name), #name);}
	#define DELEGATE_MANAGER_EVENT3(name, param1, param2, param3) void name(MyGUI::WidgetPtr _sender, param1, param2, param3){callDelegate(_sender, _sender->getUserString(#name), #name);}
	#define DELEGATE_MANAGER_EVENT4(name, param1, param2, param3, param4) void name(MyGUI::WidgetPtr _sender, param1, param2, param3, param4){callDelegate(_sender, _sender->getUserString(#name), #name);}
		/*void eventName(MyGUI::WidgetPtr _sender, [parameters])
		{
			callDelegate(_sender, _sender->getUserString("eventName"), "eventName");
		}*/

	/**	Manager for handling delegates by names.
	*/
	class MYGUI_EXPORT DelegateManager
	{
		MYGUI_INSTANCE_HEADER(DelegateManager);
	public:
		typedef delegates::CDelegate3<WidgetPtr, const std::string&, const std::string&> HandleEvent;
		typedef std::map<std::string, HandleEvent> MapDelegate;
	public:
		void initialise();
		void shutdown();

		/** @example "Add new delegate for DelegateManager"
		@code
		MyGUI::DelegateManager::getInstance().addDelegate("delegate_name_1", MyGUI::newDelegate(your_func));
		MyGUI::DelegateManager::getInstance().addDelegate("delegate_name_2", MyGUI::newDelegate(your_static_method));
		MyGUI::DelegateManager::getInstance().addDelegate("delegate_name_3", MyGUI::newDelegate(your_class_ptr, &your_class_name::your_method_name));
		@endcode

		signature : void method(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _event)
		*/
		void addDelegate(const std::string & _key, HandleEvent::IDelegate * _delegate);

		/** Remove delegate */
		void removeDelegate(const std::string & _key);
		
		/** Add delegate that called by default if there's no delegate by called key
			@note If there's no default delegate and was called event with uregistered key
		*/
		void addDefaultDelegate(HandleEvent::IDelegate * _delegate);
	private:
		void callDelegate(WidgetPtr _sender, const std::string & _key, const std::string & _event);
	public:
		DELEGATE_MANAGER_EVENT1(eventMouseLostFocus, MyGUI::WidgetPtr _new);
		DELEGATE_MANAGER_EVENT1(eventMouseSetFocus, MyGUI::WidgetPtr _new);
		DELEGATE_MANAGER_EVENT2(eventMouseDrag, int _left, int _top);
		DELEGATE_MANAGER_EVENT2(eventMouseMove, int _left, int _top);
		DELEGATE_MANAGER_EVENT1(eventMouseWheel, int _rel);
		DELEGATE_MANAGER_EVENT3(eventMouseButtonPressed, int _left, int _top, MyGUI::MouseButton _id);
		DELEGATE_MANAGER_EVENT3(eventMouseButtonReleased, int _left, int _top, MyGUI::MouseButton _id);
		DELEGATE_MANAGER_EVENT0(eventMouseButtonClick);
		DELEGATE_MANAGER_EVENT0(eventMouseButtonDoubleClick);


		///** Event : Widget lost keyboard focus.\n
		//	signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)\n
		//	@param _new widget with keyboard focus or nullptr
		//*/
		//EventHandle_WidgetWidget eventKeyLostFocus;

		///** Event : Widget got keyboard focus.\n
		//	signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)\n
		//	@param _old widget with keyboard focus or nullptr
		//*/
		//EventHandle_WidgetWidget eventKeySetFocus;

		///** Event : Key pressed.\n
		//	signature : void method(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key, MyGUI::Char _char)\n
		//	@param _key code
		//	@param _char of pressed symbol (for multilanguage applications)
		//*/
		//EventHandle_WidgetKeyCodeChar eventKeyButtonPressed;

		///** Event : Key released.\n
		//	signature : void method(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key)\n
		//	@param _key code
		//*/
		//EventHandle_WidgetKeyCode eventKeyButtonReleased;

		///** Event : Root widget changed mouse focus.\n
		//	info : this event sends only to root widget\n
		//	signature : void method(MyGUI::WidgetPtr _sender, bool _focus);
		//	@param _focus Is widget got mouse focus.
		//*/
		//EventHandle_WidgetBool  eventRootMouseChangeFocus;

		///** Event : Root widget changed keyboard focus.\n
		//	info : this event sends only to root widget\n
		//	signature : void method(MyGUI::WidgetPtr _sender, bool _focus);
		//	@param _focus Is widget got keyboard focus.
		//*/
		//EventHandle_WidgetBool eventRootKeyChangeFocus;

		///** Event : Extendeble event for special cases or plugins.\n
		//	signature : void method(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _value);
		//*/
		///* event : общее расширяемое событие для плагинов или особых случаев*/
		///* signature : void method(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _value);*/
		//EventHandle_WidgetStringString eventActionInfo;

		///* event : внутренний запрос на родителя и номера айтема, у любого виджета*/
		///* signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr & _list, size_t & _index);*/
		//EventHandle_WidgetRefWidgetRefSizeT  _requestGetContainer;

		///* event : внутреннее событие, невалидна информация для дропа*/
		///* signature : void method(MyGUI::WidgetPtr _sender);*/
		//EventHandle_WidgetVoid eventInvalideContainer;

		//// event : запрос на начало дропа
		//// signature : void method(MyGUI::WidgetPtr _sender, const MyGUI::DDItemInfo& _info, bool & _result)
		//EventHandle_WidgetCItemDropInfoRefBoolRef eventStartDrag;

		//// event : запрос на дроп айтема
		//// signature : void method(MyGUI::WidgetPtr _sender, const MyGUI::DDItemInfo& _info, bool & _result)
		//EventHandle_WidgetCItemDropInfoRefBoolRef eventRequestDrop;

		//// event : завершение дропа
		//// signature : void method(MyGUI::WidgetPtr _sender, const MyGUI::DDItemInfo& _info, bool _result)
		//EventHandle_WidgetCItemDropInfoRefBool eventDropResult;

		//// event : текущее состояние дропа
		//// signature : void method(MyGUI::WidgetPtr _sender, DropState _state)
		//EventHandle_WidgetDropState eventChangeDDState;

		DELEGATE_MANAGER_EVENT1(eventToolTip, const MyGUI::ToolTipInfo & _info);

		MapDelegate mDelegates;
		HandleEvent mDefaultDelegate;
	};

} // namespace MyGUI

#endif // __MYGUI_DELEGATE_MANAGER_H__
