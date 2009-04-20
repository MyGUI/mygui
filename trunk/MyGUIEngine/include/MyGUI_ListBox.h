/*!
	@file
	@author		Albert Semenov
	@date		04/2009
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
#ifndef __MYGUI_LIST_BOX_H__
#define __MYGUI_LIST_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ListCtrl.h"

namespace MyGUI
{

	class MYGUI_EXPORT ListBox : public ListCtrl
	{
		// ��� ������ ��������� ������������
		friend class factory::BaseWidgetFactory<ListBox>;

		MYGUI_RTTI_CHILD_HEADER( ListBox, ListCtrl );

	public:

		//------------------------------------------------------------------------------//
		// ����������� ��������

		//! Get number of items
		size_t getItemCount() { return mItemsInfo.size(); }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, const Ogre::UTFString & _name, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(const Ogre::UTFString & _name, Any _data = Any::Null) { insertItemAt(ITEM_NONE, _name, _data); }

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();

		//! Swap items at a specified positions
		void swapItemsAt(size_t _index1, size_t _index2);

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const Ogre::UTFString & _name);


		//------------------------------------------------------------------------------//
		// ����������� �����������

		/** Get index of selected item (ITEM_NONE if none selected) */
		size_t getIndexSelected() { return Base::getIndexSelected(); }

		/** Select specified _index */
		void setIndexSelected(size_t _index) { Base::setIndexSelected(_index); }

		/** Clear item selection */
		void clearIndexSelected() { Base::clearIndexSelected(); }


		//------------------------------------------------------------------------------//
		// ����������� �������

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data) { Base::setItemDataAt(_index, _data); }

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { Base::clearItemDataAt(_index); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true) { return Base::getItemDataAt<ValueType>(_index, _throw); }


		//------------------------------------------------------------------------------//
		// ����������� ������������

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const Ogre::UTFString & _name);

		//! Get item name from specified position
		const Ogre::UTFString & getItemNameAt(size_t _index);


		//------------------------------------------------------------------------------//
		// ����������� ����������

		//! Move all elements so specified becomes visible
		void beginToItemAt(size_t _index);

		//! Move all elements so first becomes visible
		void beginToItemFirst() { if (getItemCount()) beginToItemAt(0); }

		//! Move all elements so last becomes visible
		void beginToItemLast() { if (getItemCount()) beginToItemAt(getItemCount() - 1); }

		//! Move all elements so selected becomes visible
		void beginToItemSelected() { if (getIndexSelected() != ITEM_NONE) beginToItemAt(getIndexSelected()); }



	protected:
		ListBox(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~ListBox();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

		void notifyCreateWidgetItem(MyGUI::ListCtrlPtr _sender, MyGUI::WidgetPtr _item);
		void notifyDrawItem(MyGUI::ListCtrlPtr _sender, MyGUI::WidgetPtr _item, const MyGUI::IBDrawItemInfo & _info, MyGUI::IntCoord& _coord);

	private:
		std::string mSkinLine;
		int mHeightLine;

		typedef std::vector<Ogre::UTFString> VectorUTFString;
		VectorUTFString mItemsInfo;

	};

} // namespace MyGUI

#endif // __MYGUI_LIST_BOX_H__
