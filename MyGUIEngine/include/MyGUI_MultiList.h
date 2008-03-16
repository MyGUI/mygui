/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
#ifndef __MYGUI_MULTI_LIST_H__
#define __MYGUI_MULTI_LIST_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_FrameListener.h"

namespace MyGUI
{

	struct RowInfo
	{
		ListPtr list;
		ButtonPtr button;
		int width;
		Ogre::DisplayString name;
	};

	typedef std::vector<RowInfo> VectorRowInfo;
	typedef std::vector<size_t> VectorSizeT;

	class _MyGUIExport MultiList : public Widget , public FrameListener
	{
		// для вызова закрытого конструктора
		friend class factory::MultiListFactory;

	protected:
		MultiList(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~MultiList();

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {static Ogre::String type("MultiList"); return type;}
		//!	@copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		//----------------------------------------------------------------------------------//
		// методы для работы со столбцами
		inline size_t getRowCount() { return mVectorRowInfo.size();}

		void insertRow(size_t _index, int _width, const Ogre::DisplayString & _name);
		inline void addRow(int _width, const Ogre::DisplayString & _name) {insertRow(ITEM_NONE, _width, _name);}
		void setRowName(size_t _index, const Ogre::DisplayString & _name);
		void setRowWidth(size_t _index, int _width);
		const Ogre::DisplayString & getRowName(size_t _index);
		int getRowWidth(size_t _index);

		void deleteRow(size_t _index);
		void deleteAllRows();

		//----------------------------------------------------------------------------------//
		// методы для работы со строками
		size_t getItemCount();

		void insertItem(size_t _index, const Ogre::DisplayString & _item);
		inline void addItem(const Ogre::DisplayString & _item) {insertItem(ITEM_NONE, _item);}
		void setItem(size_t _index, const Ogre::DisplayString & _item);
		const Ogre::DisplayString & getItem(size_t _index);

		void deleteItem(size_t _index);
		void deleteAllItems();

		size_t getItemSelect();
		void resetItemSelect();
		void setItemSelect(size_t _index);

		//----------------------------------------------------------------------------------//
		// методы для работы с саб строками
		void setSubItem(size_t _row, size_t _index, const Ogre::DisplayString & _item);
		const Ogre::DisplayString & getSubItem(size_t _row, size_t _index);

		//----------------------------------------------------------------------------------//
		virtual void setSize(const IntSize& _size);
		virtual void setPosition(const IntCoord& _coord);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// event : нажат ентер, или двойной щелчек
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _position)
		EventInfo_WidgetSizeT eventListSelectAccept;

		// event : изменилась позиция
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _position)
		EventInfo_WidgetSizeT eventListChangePosition;

	protected:
		virtual void _frameEntered(float _frame);

		void notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyListChangeFocus(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyListChangeScrollPosition(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyButtonClick(MyGUI::WidgetPtr _widget);
		void notifyListSelectAccept(MyGUI::WidgetPtr _widget, size_t _position);

		void updateRows();
		void redrawButtons();
		void updateOnlyEmpty();

		void sortList();
		void flipList();

		void _insertSortIndex(size_t _index);
		void _eraseSortIndex(size_t _index);
		void _clearSortIndex();

		size_t convertFromSort(size_t _index);
		size_t convertToSort(size_t _index);

		void setDirtySort();

	private:
		int mHeightButton;
		int mWidthBar;
		std::string mSkinButton, mSkinList;
		ButtonPtr mButtonMain;

		WidgetPtr mWidgetClient;
		VectorRowInfo mVectorRowInfo;

		size_t mLastMouseFocusIndex;

		bool mSortUp;
		size_t mSortRowIndex;


		// векторы для быстрого маппинга в сортированном списке
		VectorSizeT mToSortIndex;
		bool mIsDirtySort;
		


	}; // class _MyGUIExport MultiList : public Widget

} // namespace MyGUI

#endif // __MYGUI_MULTI_LIST_H__
