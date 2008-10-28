/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_ITEM_DROP_INFO_H__
#define __MYGUI_ITEM_DROP_INFO_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	enum MYGUI_OBSOLETE("use DropItemState")
	//ObsoleteTypeDropState
	{
		//use DropItemState::None
		DROP_NONE,
		//use DropItemState::Start
		DROP_START,
		//use DropItemState::End
		DROP_END,
		//use DropItemState::Miss
		DROP_MISS,
		//use DropItemState::Accept
		DROP_ACCEPT,
		//use DropItemState::Refuse
		DROP_REFUSE
	};

	struct _MyGUIExport DropItemState
	{
		enum
		{
			None,
			Start,
			End,
			Miss,
			Accept,
			Refuse
		};

		int value;
		DropItemState() : value(None) { }
		DropItemState(int _value) : value(_value) { }
		friend bool operator == (DropItemState const & a, DropItemState const & b) { return a.value == b.value; }
		friend bool operator != (DropItemState const & a, DropItemState const & b) { return a.value != b.value; }
	};

	// ñòðóêòóðà èíôîðìàöèè îá èíäåêñàõ äðîïà
	struct _MyGUIExport ItemDropInfo
	{
		ItemDropInfo() :
			reseiver(null),
			reseiver_index(ITEM_NONE),
			sender(null),
			sender_index(ITEM_NONE)
		{
		}

		ItemDropInfo(WidgetPtr _sender, size_t _sender_index, WidgetPtr _reseiver, size_t _reseiver_index) :
			sender(_sender),
			sender_index(_sender_index),
			reseiver(_reseiver),
			reseiver_index(_reseiver_index)
		{
		}

		void set(WidgetPtr _sender, size_t _sender_index, WidgetPtr _reseiver, size_t _reseiver_index)
		{
			sender = _sender;
			sender_index = _sender_index;
			reseiver = _reseiver;
			reseiver_index = _reseiver_index;
		}

		void reset()
		{
			reseiver = null;
			reseiver_index = ITEM_NONE;
			sender = null;
			sender_index = ITEM_NONE;
		}

		// ïîñûëàþùèé âèäæåò 
		WidgetPtr sender;
		// èíäåêñ ïîñûëàþùåãî âèäæåòà
		size_t sender_index;

		// ïðèíèìàþùèé âèäæåò
		WidgetPtr reseiver;
		// èíäåêñ ïðèíèìàþùåãî âèäæåòà
		size_t reseiver_index;
	};

	struct _MyGUIExport DropWidgetInfo
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

	struct _MyGUIExport DropWidgetState
	{
		DropWidgetState(size_t _index) :
			index(_index),
			accept(false),
			refuse(false),
			update(true)
		{ }

		// èíäåêñ ýòîãî ýëåìåíòà
		/** Index of element */
		size_t index;
		// èçìåíèëîñü íå òîëüêî ñîñòîÿíèå, íî è ñîäåðæèìîå
		/** State and interdan data changed */
		bool update;
		// àéòåì ïðèíèìàìåò äðîï
		/** Is widget accept drag */
		bool accept;
		// àéòåì íå áåðåò äðîï
		/** Is widget refuse drag */
		bool refuse;
	};

} // namespace MyGUI

#endif // __MYGUI_ITEM_DROP_INFO_H__
