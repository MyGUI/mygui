/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_COMBO_BOX_H__
#define __MYGUI_COMBO_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Edit.h"

namespace MyGUI
{

	class _MyGUIExport ComboBox : public Edit
	{
		// для вызова закрытого конструктора
		friend class factory::ComboBoxFactory;

	protected:
		ComboBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		~ComboBox();

		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		//------------------------------------------------------------------------------------//
		// методы для манипуляций строками
		//! Get number of items
		size_t getItemCount();

		//! Insert an item into a list at specified position
		void insertItem(size_t _index, const Ogre::UTFString & _item);
		//! Add an item to the end of a list
		void addItem(const Ogre::UTFString & _item);
		//! Replace an item at a specified position
		void setItem(size_t _index, const Ogre::UTFString & _item);
		//! Get item from specified position
		const Ogre::UTFString & getItem(size_t _index);

		//! Delete item at a specified position
		void deleteItem(size_t _index);
		//! Delete all items
		void deleteAllItems();

		//! Get number of selected item (ITEM_NONE if none selected)
		inline size_t getItemSelect() {return mItemIndex;}
		//! Reset item selection
		inline void resetItemSelect() {setItemSelect(ITEM_NONE);}
		//! Select specified _index
		void setItemSelect(size_t _index);

		//------------------------------------------------------------------------------------//
		// методы для управления отображением
		//! Set drop list mode (text can not be edited)
		void setComboModeDrop(bool _drop);
		//! Get drop list mode flag
		inline bool getComboModeDrop() { return mModeDrop; }

		//! Set smooth show of list
		inline void setSmoothShow(bool _smooth) { mShowSmooth = _smooth; }
		//! Get smooth show of list flag
		inline bool getSmoothShow() { return mShowSmooth; }

		//! Set max list height
		inline int getMaxListHeight() { return mMaxHeight; }
		//! Get max list height
		inline void setMaxListHeight(int _height) { mMaxHeight = _height; }

		//------------------------------------------------------------------------------------//

		/** Event : Enter pressed in combo mode or item selected in drop.\n
			signature : void method(MyGUI::WidgetPtr _sender)
		*/
		EventInfo_WidgetVoid eventComboAccept;

		/** Event : Position changed.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of new position
		*/
		EventInfo_WidgetSizeT eventComboChangePosition;

	protected:
		virtual void _onKeyButtonPressed(KeyCode _key, Char _char);

		void notifyButtonPressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyListLostFocus(WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyListSelectAccept(WidgetPtr _widget, size_t _position);
		void notifyListMouseItemActivate(WidgetPtr _widget, size_t _position);
		void notifyListChangePosition(WidgetPtr _widget, size_t _position);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);
		void notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyEditTextChange(WidgetPtr _sender);

		void showList();
		void hideList();

	private:
		ButtonPtr mButton;
		ListPtr mList;

		bool mListShow;
		int mMaxHeight;
		size_t mItemIndex;
		bool mModeDrop;
		bool mDropMouse;
		bool mShowSmooth;

		//float mDoAlpha;

	}; // class _MyGUIExport ComboBox : public Edit

} // namespace MyGUI

#endif // __MYGUI_COMBO_BOX_H__
