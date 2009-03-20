/*!
	@file
	@author		Albert Semenov
	@date		10/2008
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
#ifndef __MYGUI_DDCONTAINER_H__
#define __MYGUI_DDCONTAINER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_DDItemInfo.h"
#include "MyGUI_EventPair.h"

namespace MyGUI
{

	typedef delegates::CDelegate1<DDContainerPtr> EventHandle_DDContainerPtr;
	typedef delegates::CDelegate3<DDContainerPtr, WidgetPtr&, IntCoord&> EventHandle_DDContainerPtrWidgetPtrRefIntCoordRef;
	typedef delegates::CDelegate3<DDContainerPtr, WidgetPtr, const DDWidgetState&> EventHandle_DDContainerPtrWidgetPtrCDDWidgetStateRef;
	typedef delegates::CDelegate3<DDContainerPtr, const DDItemInfo&, bool&> EventHandle_DDContainerPtrCDDItemInfoRefBoolRef;
	typedef delegates::CDelegate3<DDContainerPtr, const DDItemInfo&, bool> EventHandle_DDContainerPtrCDDItemInfoRefBool;
	typedef delegates::CDelegate2<DDContainerPtr, DDItemState> EventHandle_DDContainerPtrDDItemState;


	class MYGUI_EXPORT DDContainer : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<DDContainer>;

		MYGUI_RTTI_CHILD_HEADER( DDContainer, Widget );

	public:

		/** Set drag'n'drop mode flag.
			Disabled (false) by default.
		*/
		void setNeedDragDrop(bool _need) { mNeedDragDrop = _need; }
		/** Get drag'n'drop mode flag */
		bool getNeedDragDrop() { return mNeedDragDrop; }

	/*event:*/
		/** Event : request for start drag
			signature : void method(MyGUI::DDContainerPtr _sender, const MyGUI::DDItemInfo& _info, bool & _result)
			@param _sender widget that called this event
			@param _info information about DDContainers
			@param _result write here true if container can be draggedor false if it can't
		*/
		EventHandle_DDContainerPtrCDDItemInfoRefBoolRef eventStartDrag;

		/** Event : request for start drag (moving mouse over container, but not dropped yet)
			signature : void method(MyGUI::DDContainerPtr _sender, const MyGUI::DDItemInfo& _info, bool & _result)
			@param _sender widget that called this event
			@param _info information about DDContainers
			@param _result write here true if container accept dragged widget or false if it isn't
		*/
		EventHandle_DDContainerPtrCDDItemInfoRefBoolRef eventRequestDrop;

		/** Event : end drag (drop)
			signature : void method(MyGUI::DDContainerPtr _sender, const MyGUI::DDItemInfo& _info, bool _result)
			@param _sender widget that called this event
			@param _info information about DDContainers
			@param _result if true then drop was successfull
		*/
		EventHandle_DDContainerPtrCDDItemInfoRefBool eventDropResult;

		/** Event : drag'n'drop state changed
			signature : void method(MyGUI::DDContainerPtr _sender, MyGUI::DDItemState _state)
			@param _sender widget that called this event
			@param _state new state
		*/
		EventHandle_DDContainerPtrDDItemState eventChangeDDState;

		/** Event : [not used] request widget for dragging
			signature : void method(MyGUI::DDContainerPtr _sender, MyGUI::WidgetPtr& _item, MyGUI::IntCoord& _dimension)
			@param _sender widget that called this event
			@param _item write widget pointer here
			@param _dimension write widget coordinate here
		*/
		EventHandle_DDContainerPtrWidgetPtrRefIntCoordRef requestDragWidgetInfo;


	/*internal:*/
		// метод для установления стейта айтема
		virtual void _setContainerItemInfo(size_t _index, bool _set, bool _accept) { }

		/** Event : внутреннее событие, невалидна информация для контейнера
			signature : void method(MyGUI::DDContainerPtr _sender)
			@param _sender widget that called this event
		*/
		EventPair<EventHandle_WidgetVoid, EventHandle_DDContainerPtr> _eventInvalideContainer;

		/** Event : !!обновить виджеты дропа DD_FIXME наверное internal
			signature : void method(MyGUI::DDContainerPtr _sender, MyGUI::WidgetPtr _item, const MyGUI::DDWidgetState & _state)
			@param _sender widget that called this event
			@param _items
			@param _state
		*/
		EventHandle_DDContainerPtrWidgetPtrCDDWidgetStateRef eventUpdateDropState;

	protected:
		DDContainer(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~DDContainer();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMouseDrag(int _left, int _top);

		virtual void notifyInvalideDrop(DDContainerPtr _sender);

		virtual void _getContainer(WidgetPtr & _container, size_t & _index);

		virtual void removeDropItems();
		virtual void updateDropItems();
		virtual void updateDropItemsState(const DDWidgetState & _state);

		void mouseDrag();
		void mouseButtonReleased(MouseButton _id);
		void mouseButtonPressed(MouseButton _id);

		void endDrop(bool _reset);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();


	protected:
		bool mDropResult;
		bool mNeedDrop;
		bool mStartDrop;

		WidgetPtr mOldDrop;
		WidgetPtr mCurrentSender;

		DDItemInfo mDropInfo;

		size_t mDropSenderIndex;

		// список виджетов для дропа
		WidgetPtr mDropItem;
		IntCoord mDropDimension;

		IntPoint mClickInWidget;

		// нужно и виджету поддержка драг энд дропа
		bool mNeedDragDrop;

		DDContainerPtr mReseiverContainer;

	};

} // namespace MyGUI

#endif // __MYGUI_DDCONTAINER_H__
