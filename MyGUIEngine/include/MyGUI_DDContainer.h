/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
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

		/** Set drag'n'drop mode flag */
		void setNeedDragDrop(bool _need) { mNeedDragDrop = _need; }
		/** Get drag'n'drop mode flag */
		bool getNeedDragDrop() { return mNeedDragDrop; }

	/*event:*/
		/** Event : !!запрос на начало драга
			signature : void method(MyGUI::DDContainerPtr _sender, const MyGUI::DDItemInfo& _info, bool & _result)
			@param _sender widget that called this event
			@param _info
			@param _result
		*/
		EventHandle_DDContainerPtrCDDItemInfoRefBoolRef eventStartDrag;

		/** Event : !!запрос на дроп айтема (навели мышой, но еще не отпустили)
			signature : void method(MyGUI::DDContainerPtr _sender, const MyGUI::DDItemInfo& _info, bool & _result)
			@param _sender widget that called this event
			@param _info
			@param _result
		*/
		EventHandle_DDContainerPtrCDDItemInfoRefBoolRef eventRequestDrop;

		/** Event : !!завершение драга (отпустили)
			signature : void method(MyGUI::DDContainerPtr _sender, const MyGUI::DDItemInfo& _info, bool _result)
			@param _sender widget that called this event
			@param _info
			@param _result
		*/
		EventHandle_DDContainerPtrCDDItemInfoRefBool eventDropResult;

		/** Event : !!смена состояния драг эн дропа
			signature : void method(MyGUI::DDContainerPtr _sender, MyGUI::DDItemState _state)
			@param _sender widget that called this event
			@param _state
		*/
		EventHandle_DDContainerPtrDDItemState eventChangeDDState;

		/** Event : !!запрашиваем виджет для драга
			signature : void method(MyGUI::DDContainerPtr _sender, MyGUI::WidgetPtr& _item, MyGUI::IntCoord& _dimension)
			@param _sender widget that called this event
			@param _items
			@param _dimension
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
