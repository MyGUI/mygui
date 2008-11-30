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

namespace MyGUI
{

	typedef delegates::CDelegate2<WidgetPtr, VectorDropWidgetInfo&> EventInfo_WidgetRefDropWidgetInfo;
	typedef delegates::CDelegate3<WidgetPtr, VectorDropWidgetInfo&, const DropWidgetState&> EventInfo_WidgetRefDropWidgetInfoState;

	// делегаты для дропа
	typedef delegates::CDelegate3<WidgetPtr, const ItemDropInfo&, bool&> EventInfo_WidgetCItemDropInfoRefBoolRef;
	typedef delegates::CDelegate3<WidgetPtr, const ItemDropInfo&, bool> EventInfo_WidgetCItemDropInfoRefBool;
	typedef delegates::CDelegate2<WidgetPtr, DropItemState> EventInfo_WidgetDropState;

	class _MyGUIExport DDContainer : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<DDContainer>;

		MYGUI_RTTI_CHILD_HEADER;

	public:

		/** Set drag'n'drop mode flag */
		void setNeedDragDrop(bool _need) { mNeedDragDrop = _need; }
		/** Get drag'n'drop mode flag */
		bool getNeedDragDrop() { return mNeedDragDrop; }

		// метод для установления стейта айтема
		virtual void setContainerItemInfo(size_t _index, bool _set, bool _accept) { }


		// event : запрос на начало дропа
		// signature : void method(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo& _info, bool & _result)
		EventInfo_WidgetCItemDropInfoRefBoolRef eventStartDrop;

		// event : запрос на дроп айтема
		// signature : void method(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo& _info, bool & _result)
		EventInfo_WidgetCItemDropInfoRefBoolRef eventRequestDrop;

		// event : завершение дропа
		// signature : void method(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo& _info, bool _result)
		EventInfo_WidgetCItemDropInfoRefBool eventEndDrop;

		// event : текущее состояние дропа
		// signature : void method(MyGUI::WidgetPtr _sender, DropState _state)
		EventInfo_WidgetDropState eventDropState;

		// event : запрашиваем виджеты для дропа
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::VectorDropWidgetInfo & _items)
		EventInfo_WidgetRefDropWidgetInfo requestDropWidgetInfo;

		// event : обновить виджеты дропа
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::VectorDropWidgetInfo & _items, const MyGUI::DropWidgetState & _state)
		EventInfo_WidgetRefDropWidgetInfoState eventUpdateDropState;

		/* event : внутреннее событие, невалидна информация для контейнера*/
		/* signature : void method(MyGUI::WidgetPtr _sender);*/
		EventInfo_WidgetVoid eventInvalideContainer;

	protected:
		DDContainer(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~DDContainer();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMouseDrag(int _left, int _top);

		virtual void notifyInvalideDrop(WidgetPtr _sender);

		virtual void getContainer(WidgetPtr & _container, size_t & _index);

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
