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
#include "MyGUI_ItemDropInfo.h"
#include "MyGUI_EventPair.h"

namespace MyGUI
{

	//OBSOLETE
	typedef delegates::CDelegate2<WidgetPtr, VectorDropWidgetInfo&> EventHandle_WidgetRefDropWidgetInfo;
	typedef delegates::CDelegate3<WidgetPtr, VectorDropWidgetInfo&, const DropWidgetState&> EventHandle_WidgetRefDropWidgetInfoState;
	typedef delegates::CDelegate3<WidgetPtr, const ItemDropInfo&, bool&> EventHandle_WidgetCItemDropInfoRefBoolRef;
	typedef delegates::CDelegate3<WidgetPtr, const ItemDropInfo&, bool> EventHandle_WidgetCItemDropInfoRefBool;
	typedef delegates::CDelegate2<WidgetPtr, DropItemState> EventHandle_WidgetDropState;

	typedef delegates::CDelegate1<DDContainerPtr> EventHandle_DDContainerPtr;
	typedef delegates::CDelegate2<DDContainerPtr, VectorDropWidgetInfo&> EventHandle_DDContainerPtrDropWidgetInfoRef;
	typedef delegates::CDelegate3<DDContainerPtr, VectorDropWidgetInfo&, const DropWidgetState&> EventHandle_DDContainerPtrDropWidgetInfoRefCDropWidgetStateRef;
	typedef delegates::CDelegate3<DDContainerPtr, const ItemDropInfo&, bool&> EventHandle_DDContainerPtrCItemDropInfoRefBoolRef;
	typedef delegates::CDelegate3<DDContainerPtr, const ItemDropInfo&, bool> EventHandle_DDContainerPtrCItemDropInfoRefBool;
	typedef delegates::CDelegate2<DDContainerPtr, DropItemState> EventHandle_DDContainerPtrDropState;


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

		// метод для установления стейта айтема
		virtual void _setContainerItemInfo(size_t _index, bool _set, bool _accept) { }


	/*event:*/
		/** Event : запрос на начало дропа
			signature : void method(MyGUI::DDContainerPtr _sender, const MyGUI::ItemDropInfo& _info, bool & _result)
			@param _sender widget that called this event
			@param _info
			@param _result
		*/
		EventPair<EventHandle_WidgetCItemDropInfoRefBoolRef, EventHandle_DDContainerPtrCItemDropInfoRefBoolRef> eventStartDrop;

		/** Event : запрос на дроп айтема
			signature : void method(MyGUI::DDContainerPtr _sender, const MyGUI::ItemDropInfo& _info, bool & _result)
			@param _sender widget that called this event
			@param _info
			@param _result
		*/
		EventPair<EventHandle_WidgetCItemDropInfoRefBoolRef, EventHandle_DDContainerPtrCItemDropInfoRefBoolRef > eventRequestDrop;

		/** Event : завершение дропа
			signature : void method(MyGUI::DDContainerPtr _sender, const MyGUI::ItemDropInfo& _info, bool _result)
			@param _sender widget that called this event
			@param _info
			@param _result
		*/
		EventPair<EventHandle_WidgetCItemDropInfoRefBool, EventHandle_DDContainerPtrCItemDropInfoRefBool> eventEndDrop;

		/** Event : текущее состояние дропа
			signature : void method(MyGUI::DDContainerPtr _sender, MyGUI::DropState _state)
			@param _sender widget that called this event
			@param _state
		*/
		EventPair<EventHandle_WidgetDropState, EventHandle_DDContainerPtrDropState> eventDropState;

		/** Event : запрашиваем виджеты для дропа
			signature : void method(MyGUI::DDContainerPtr _sender, MyGUI::VectorDropWidgetInfo & _items)
			@param _sender widget that called this event
			@param _items
		*/
		EventPair<EventHandle_WidgetRefDropWidgetInfo, EventHandle_DDContainerPtrDropWidgetInfoRef> requestDropWidgetInfo;

		/** Event : обновить виджеты дропа
			signature : void method(MyGUI::DDContainerPtr _sender, MyGUI::VectorDropWidgetInfo & _items, const MyGUI::DropWidgetState & _state)
			@param _sender widget that called this event
			@param _items
			@param _state
		*/
		EventPair<EventHandle_WidgetRefDropWidgetInfoState, EventHandle_DDContainerPtrDropWidgetInfoRefCDropWidgetStateRef> eventUpdateDropState;

		/** Event : внутреннее событие, невалидна информация для контейнера
			signature : void method(MyGUI::DDContainerPtr _sender)
			@param _sender widget that called this event
		*/
		EventPair<EventHandle_WidgetVoid, EventHandle_DDContainerPtr> eventInvalideContainer;

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
		virtual void updateDropItemsState(const DropWidgetState & _state);

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

		ItemDropInfo mDropInfo;

		size_t mDropSenderIndex;

		// список виджетов для дропа
		VectorDropWidgetInfo mDropItems;

		IntPoint mClickInWidget;

		// нужно и виджету поддержка драг энд дропа
		bool mNeedDragDrop;

		DDContainerPtr mReseiverContainer;

	};

} // namespace MyGUI

#endif // __MYGUI_DDCONTAINER_H__
