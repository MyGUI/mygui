/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
/*
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

	typedef delegates::CMultiDelegate3<DDContainer*, const DDItemInfo&, bool&> EventHandle_DDContainerPtrCDDItemInfoRefBoolRef;
	typedef delegates::CMultiDelegate3<DDContainer*, const DDItemInfo&, bool> EventHandle_DDContainerPtrCDDItemInfoRefBool;
	typedef delegates::CMultiDelegate2<DDContainer*, DDItemState> EventHandle_EventHandle_DDContainerPtrDDItemState;
	typedef delegates::CDelegate3<DDContainer*, Widget*&, IntCoord&> EventHandle_EventHandle_DDContainerPtrWidgetPtrRefIntCoordRef;


	class MYGUI_EXPORT DDContainer :
		public Widget
	{
		MYGUI_RTTI_DERIVED( DDContainer )

	public:
		DDContainer();

		/** Set drag'n'drop mode flag.
			Disabled (false) by default.
		*/
		void setNeedDragDrop(bool _value);
		/** Get drag'n'drop mode flag */
		bool getNeedDragDrop() const;

		void resetDrag();

		/*events:*/
		/** Event : Request for start drag.\n
			signature : void method(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result)
			@param _sender widget that called this event
			@param _info information about DDContainers
			@param _result write here true if container can be draggedor false if it can't
		*/
		EventHandle_DDContainerPtrCDDItemInfoRefBoolRef
			eventStartDrag;

		/** Event : Request for start drop (moving mouse over container, but not dropped yet).\n
			signature : void method(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result)
			@param _sender widget that called this event
			@param _info information about DDContainers
			@param _result write here true if container accept dragged widget or false if it isn't
		*/
		EventHandle_DDContainerPtrCDDItemInfoRefBoolRef
			eventRequestDrop;

		/** Event : End drag (drop).\n
			signature : void method(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool _result)
			@param _sender widget that called this event
			@param _info information about DDContainers
			@param _result if true then drop was successfull
		*/
		EventHandle_DDContainerPtrCDDItemInfoRefBool
			eventDropResult;

		/** Event : Drag'n'drop state changed.\n
			signature : void method(MyGUI::DDContainer* _sender, MyGUI::DDItemState _state)
			@param _sender widget that called this event
			@param _state new state
		*/
		EventHandle_EventHandle_DDContainerPtrDDItemState
			eventChangeDDState;

		/** Event : [not used] Request widget for dragging.\n
			signature : void method(MyGUI::DDContainer* _sender, MyGUI::Widget*& _item, MyGUI::IntCoord& _dimension)
			@param _sender widget that called this event
			@param _item write widget pointer here
			@param _dimension write widget coordinate here
		*/
		EventHandle_EventHandle_DDContainerPtrWidgetPtrRefIntCoordRef
			requestDragWidgetInfo;


		/*internal:*/
		// метод для установления стейта айтема
		virtual void _setContainerItemInfo(size_t _index, bool _set, bool _accept);

		/** Event : [Internal event] невалидна информация для контейнера.\n
			signature : void method(MyGUI::DDContainer* _sender)
			@param _sender widget that called this event
		*/
		delegates::CMultiDelegate1<DDContainer*>
			_eventInvalideContainer;

		/** Event : [Internal event] !!обновить виджеты дропа DD_FIXME наверное internal.\n
			signature : void method(MyGUI::DDContainer* _sender, MyGUI::Widget* _item, const MyGUI::DDWidgetState& _state)
			@param _sender widget that called this event
			@param _items
			@param _state
		*/
		delegates::CMultiDelegate3<DDContainer*, Widget*, const DDWidgetState&>
			eventUpdateDropState;

	protected:
		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMouseDrag(int _left, int _top, MouseButton _id);

		virtual void notifyInvalideDrop(DDContainer* _sender);

		virtual void removeDropItems();
		virtual void updateDropItems();
		virtual void updateDropItemsState(const DDWidgetState& _state);

		void mouseDrag(MouseButton _id);
		void mouseButtonReleased(MouseButton _id);
		void mouseButtonPressed(MouseButton _id);

		void endDrop(bool _reset);

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	protected:
		bool mDropResult;
		bool mNeedDrop;
		bool mStartDrop;

		Widget* mOldDrop;
		Widget* mCurrentSender;

		DDItemInfo mDropInfo;

		size_t mDropSenderIndex;

		// список виджетов для дропа
		Widget* mDropItem;
		IntCoord mDropDimension;

		IntPoint mClickInWidget;

		// нужно и виджету поддержка драг энд дропа
		bool mNeedDragDrop;

		DDContainer* mReseiverContainer;
	};

} // namespace MyGUI

#endif // __MYGUI_DDCONTAINER_H__
