/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_COMBO_BOX_H_
#define MYGUI_COMBO_BOX_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_EditBox.h"
#include "MyGUI_ListBox.h"
#include "MyGUI_Any.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_FlowDirection.h"
#include "MyGUI_IItem.h"
#include "MyGUI_IItemContainer.h"

namespace MyGUI
{

	typedef delegates::CMultiDelegate2<ComboBox*, size_t> EventHandle_ComboBoxPtrSizeT;

	/** \brief @wpage{ComboBox}
		ComboBox widget description should be here.
	*/
	class MYGUI_EXPORT ComboBox :
		public EditBox,
		public IItemContainer,
		public MemberObsolete<ComboBox>
	{
		MYGUI_RTTI_DERIVED( ComboBox )

	public:
		ComboBox();

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() const;

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, const UString& _name, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(const UString& _name, Any _data = Any::Null);

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();


		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const UString& _name);


		//------------------------------------------------------------------------------//
		// манипуляции выделениями

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getIndexSelected() const;

		//! Select specified _index
		void setIndexSelected(size_t _index);

		//! Clear item selection
		void clearIndexSelected();


		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index);

		//! Get item data from specified position
		template <typename ValueType>
		ValueType* getItemDataAt(size_t _index, bool _throw = true)
		{
			return mList->getItemDataAt<ValueType>(_index, _throw);
		}


		//------------------------------------------------------------------------------//
		// манипуляции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const UString& _name);

		//! Get item name from specified position
		const UString& getItemNameAt(size_t _index);


		//------------------------------------------------------------------------------//
		// манипуляции выдимостью

		//! Move all elements so specified becomes visible
		void beginToItemAt(size_t _index);

		//! Move all elements so first becomes visible
		void beginToItemFirst();

		//! Move all elements so last becomes visible
		void beginToItemLast();

		//! Move all elements so selected becomes visible
		void beginToItemSelected();


		//------------------------------------------------------------------------------------//
		// методы для управления отображением

		//! Set drop list mode (text can not be edited)
		void setComboModeDrop(bool _value);
		//! Get drop list mode flag
		bool getComboModeDrop() const;

		//! Set smooth show of list
		void setSmoothShow(bool _value);
		//! Get smooth show of list flag
		bool getSmoothShow() const;

		//! Get max list length
		void setMaxListLength(int _value);
		//! Set max list length
		int getMaxListLength() const;

		// RENAME
		//! Set direction, where drop down list appears (TopToBottom by default).
		void setFlowDirection(FlowDirection _value);
		//! Get direction, where drop down list appears.
		FlowDirection getFlowDirection() const;

		/*events:*/
		/** Event : Enter pressed in combo mode or item selected in drop down list
			and combo mode drop enabled (see void ComboBox::setComboModeDrop(bool _value)).\n
			signature : void method(MyGUI::ComboBox* _sender, size_t _index)
			@param _sender widget that called this event
			@param _index item
		*/
		EventPair<EventHandle_WidgetVoid, EventHandle_ComboBoxPtrSizeT> eventComboAccept;

		/** Event : Position changed.\n
			signature : void method(MyGUI::ComboBox* _sender, size_t _index)
			@param _sender widget that called this event
			@param _index of new position
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ComboBoxPtrSizeT> eventComboChangePosition;

		/*internal:*/
		// IItemContainer impl
		virtual size_t _getItemCount();
		virtual void _addItem(const MyGUI::UString& _name);
		virtual void _removeItemAt(size_t _index);
		virtual void _setItemNameAt(size_t _index, const UString& _name);
		virtual const UString& _getItemNameAt(size_t _index);

		virtual void _resetContainer(bool _update);

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		virtual void onKeyButtonPressed(KeyCode _key, Char _char);

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		void notifyButtonPressed(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyListLostFocus(Widget* _sender, MyGUI::Widget* _new);
		void notifyListSelectAccept(ListBox* _widget, size_t _position);
		void notifyListMouseItemActivate(ListBox* _widget, size_t _position);
		void notifyListChangePosition(ListBox* _widget, size_t _position);
		void notifyMouseWheel(Widget* _sender, int _rel);
		void notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyEditTextChange(EditBox* _sender);
		void notifyToolTip(Widget* _sender, const ToolTipInfo& _info);

		void showList();
		void hideList();

		void actionWidgetHide(Widget* _widget, ControllerItem* _controller);

		ControllerFadeAlpha* createControllerFadeAlpha(float _alpha, float _coef, bool _enable);
		IntCoord calculateListPosition();

	private:
		Button* mButton;
		ListBox* mList;

		bool mListShow;
		int mMaxListLength;
		size_t mItemIndex;
		bool mModeDrop;
		bool mDropMouse;
		bool mShowSmooth;

		FlowDirection mFlowDirection;
	};

} // namespace MyGUI

#endif // MYGUI_COMBO_BOX_H_
