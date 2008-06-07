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

	struct ColumnInfo
	{
		ListPtr list;
		ButtonPtr button;
		int width;
		Ogre::UTFString name;
	};

	typedef std::vector<ColumnInfo> VectorColumnInfo;
	typedef std::vector<size_t> VectorSizeT;

	class _MyGUIExport MultiList : public Widget , public FrameListener
	{
		// для вызова закрытого конструктора
		friend class factory::MultiListFactory;

		enum ImageSort
		{
			SORT_NONE,
			SORT_UP,
			SORT_DOWN
		};

	protected:
		MultiList(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~MultiList();

		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		//----------------------------------------------------------------------------------//
		// Methods for work with columns (RU:методы для работы со столбцами)
		//! Get number of columns
		inline size_t getColumnCount() { return mVectorColumnInfo.size();}

		/** Insert new column
			@param _column New column will be inserted before _column
			@param _width Width of new column
			@param _name Name of new column
		*/
		void insertColumn(size_t _column, int _width, const Ogre::UTFString & _name);
		/** Add new column at last position
			@param _width Width of new column
			@param _name Name of new column
		*/
		inline void addColumn(int _width, const Ogre::UTFString & _name) {insertColumn(ITEM_NONE, _width, _name);}
		/** Set column name
			@param _column Index of column
			@param _name New name of column
		*/
		void setColumnName(size_t _column, const Ogre::UTFString & _name);
		/** Set column width
			@param _column Index of column
			@param _name New width of column
		*/
		void setColumnWidth(size_t _column, int _width);
		/** Get _column name */
		const Ogre::UTFString & getColumnName(size_t _column);
		/** Get _column width */
		int getColumnWidth(size_t _column);

		/** Delete column */
		void deleteColumn(size_t _column);
		/** Delete all columns */
		void deleteAllColumns();

		//----------------------------------------------------------------------------------//
		// Methods for work with lines (RU:методы для работы со строками)
		/** Get number of items (lines) */
		size_t getItemCount();

		/** Insert new item before _index line */
		void insertItem(size_t _index, const Ogre::UTFString & _item);
		/** Add new item at the end */
		inline void addItem(const Ogre::UTFString & _item) {insertItem(ITEM_NONE, _item);}
		/** Set item string */
		void setItem(size_t _index, const Ogre::UTFString & _item);
		/** Get item string */
		const Ogre::UTFString & getItem(size_t _index);

		/** Delete item */
		void deleteItem(size_t _index);
		/** Delete all items */
		void deleteAllItems();

		/** Get index of selected item (ITEM_NONE if none selected) */
		size_t getItemSelect();
		/** Reset item selection */
		void resetItemSelect();
		/** Select specified _index */
		void setItemSelect(size_t _index);

		//----------------------------------------------------------------------------------//
		// Methods for work with sub lines (RU:методы для работы со саб строками)
		/** Set sub item
			@param _column Index of column
			@param _index Index of line
			@param _item New sub item value
		*/
		void setSubItem(size_t _column, size_t _index, const Ogre::UTFString & _item);
		/** Get sub item */
		const Ogre::UTFString & getSubItem(size_t _column, size_t _index);
		/** Search item in specified _column, returns index of the first occurrence in column or ITEM_NONE if item not found */
		size_t findItem(size_t _column, const Ogre::UTFString & _item);
		//----------------------------------------------------------------------------------//

		/** Event : Enter pressed or double click.\n
			signature : void method(WidgetPtr _sender, size_t _index)\n
			@param _index of selected item
		*/
		EventInfo_WidgetSizeT eventListSelectAccept;

		/** Event : Selected item position changed.\n
			signature : void method(WidgetPtr _sender, size_t _index)\n
			@param _index of new item
		*/
		EventInfo_WidgetSizeT eventListChangePosition;

	protected:
		virtual void _frameEntered(float _frame);

		void notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyListChangeFocus(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyListChangeScrollPosition(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyButtonClick(MyGUI::WidgetPtr _widget);
		void notifyListSelectAccept(MyGUI::WidgetPtr _widget, size_t _position);

		void updateColumns();
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

		WidgetPtr getSeparator(size_t _index);

	private:
		int mHeightButton;
		int mWidthBar;
		std::string mSkinButton, mSkinList;
		ButtonPtr mButtonMain;

		//WidgetPtr mWidgetClient;
		VectorColumnInfo mVectorColumnInfo;

		VectorWidgetPtr mSeparators;

		size_t mLastMouseFocusIndex;

		bool mSortUp;
		size_t mSortColumnIndex;


		// векторы для быстрого маппинга в сортированном списке
		VectorSizeT mToSortIndex;
		bool mIsDirtySort;

		int mWidthSeparator;
		std::string mSkinSeparator;
		int mOffsetButtonSeparator;

	}; // class _MyGUIExport MultiList : public Widget

} // namespace MyGUI

#endif // __MYGUI_MULTI_LIST_H__
