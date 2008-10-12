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

namespace MyGUI
{

	struct DropWidgetInfo
	{
		DropWidgetInfo(WidgetPtr _item, const IntCoord & _dimension) :
			item(_item),
			dimension(_dimension)
		{
		}

		WidgetPtr item;
		IntCoord dimension;
	};
	typedef std::vector<DropWidgetInfo> VectorDropWidgetInfo;

	struct DropWidgetState
	{
		DropWidgetState(size_t _index) :
			index(_index),
			accept(false),
			refuse(false)
		{ }

		// индекс этого элемента
		/** Index of element */
		size_t index;
		// айтем принимамет дроп
		/** Is widget accept drag */
		bool accept;
		// айтем не берет дроп
		/** Is widget refuse drag */
		bool refuse;
	};

	typedef delegates::CDelegate2<WidgetPtr, VectorDropWidgetInfo&> EventInfo_WidgetRefDropWidgetInfo;
	typedef delegates::CDelegate3<WidgetPtr, VectorDropWidgetInfo&, const DropWidgetState&> EventInfo_WidgetRefDropWidgetInfoState;

	class _MyGUIExport DDContainer : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::DDContainerFactory;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		DDContainer(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMouseDrag(int _left, int _top);

		virtual void notifyInvalideDrop(WidgetPtr _sender);

		void mouseDrag();
		void mouseButtonReleased(MouseButton _id);
		void mouseButtonPressed(MouseButton _id);

		void endDrop(bool _reset);

		virtual void removeDropItems();
		virtual void updateDropItems();
		virtual void updateDropItemsState(const DropWidgetState & _state);

		virtual void getContainer(WidgetPtr & _container, size_t & _index);

	public:

		// event : запрашиваем виджеты для дропа
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::VectorDropWidgetInfo & _items)
		EventInfo_WidgetRefDropWidgetInfo requestDropWidgetInfo;

		// event : обновить виджеты дропа
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::VectorDropWidgetInfo & _items, const MyGUI::DropWidgetState & _state)
		EventInfo_WidgetRefDropWidgetInfoState eventUpdateDropState;

	private:
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

	};

} // namespace MyGUI

#endif // __MYGUI_DDCONTAINER_H__
