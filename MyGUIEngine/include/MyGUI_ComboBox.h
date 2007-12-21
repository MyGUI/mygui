/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_COMBO_BOX_H__
#define __MYGUI_COMBO_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ComboBoxFactory.h"
#include "MyGUI_Edit.h"
#include "MyGUI_List.h"

namespace MyGUI
{

	class ComboBox;
	typedef ComboBox* ComboBoxPtr;

	class _MyGUIExport ComboBox : public Edit
	{
		// для вызова закрытого конструктора
		friend class factory::ComboBoxFactory;

	protected:
		ComboBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

		virtual void _onKeyLostFocus(WidgetPtr _new);
		virtual void _onKeySetFocus(WidgetPtr _old);
		virtual void _onKeyButtonPressed(int _key, wchar_t _char);
		virtual void _frameEntered(float _frame);

		void notifyButtonPressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyListLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyListSelectAccept(MyGUI::WidgetPtr _widget);
		void notifyListMouseChangePosition(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyEditTextChange(MyGUI::WidgetPtr _sender);

		void showList();
		void hideList();

		/*virtual void setPosition(const IntPoint& _pos);
		virtual void setPosition(const IntCoord& _coord);
		virtual void setSize(const IntSize& _size);*/

	public:
		// тип данного виджета
		inline static const Ogre::String & getType() {static Ogre::String type("ComboBox"); return type;}

		inline void setComboItemIndex(size_t _index)
		{
			MYGUI_ASSERT(_index < mList->getItemCount());
			mItemIndex = _index;
			Edit::setCaption(mList->getItemString(_index));
		}

		inline size_t getComboItemIndex()
		{
			return mItemIndex;
		}

		virtual void setCaption(const Ogre::DisplayString& _text)
		{
			if (false == mModeDrop) Edit::setCaption(_text);
		}

		inline void setComboModeDrop(bool _drop)
		{
			mModeDrop = _drop;
			setEditStatic(mModeDrop);
		}

		inline bool getComboModeDrop()
		{
			return mModeDrop;
		}

		inline size_t getItemCount()
		{
			return mList->getItemCount();
		}

		inline const Ogre::DisplayString & getItemString(size_t _index)
		{
			MYGUI_ASSERT(_index < mList->getItemCount());
			return mList->getItemString(_index);
		}

		inline void setItemString(size_t _index, const Ogre::DisplayString & _item)
		{
			MYGUI_ASSERT(_index < mList->getItemCount());
			mList->setItemString(_index, _item);
			mItemIndex = ITEM_NONE;
			mList->setItemSelect(mItemIndex);
		}

		void insertItemString(size_t _index, const Ogre::DisplayString & _item)
		{
			mList->insertItemString(_index, _item);
			mItemIndex = ITEM_NONE;
			mList->setItemSelect(mItemIndex);
		}

		void deleteItemString(size_t _index)
		{
			mList->deleteItemString(_index);
			mItemIndex = ITEM_NONE;
			mList->setItemSelect(mItemIndex);
		}

		inline void addItemString(const Ogre::DisplayString& _item)
		{
			mList->addItemString(_item);
			mItemIndex = ITEM_NONE;
			mList->setItemSelect(mItemIndex);
		}

		void setSmoothShow(bool _smooth);
		inline bool getSmoothShow()
		{
			return mShowSmooth;
		}

		// event : нажат энтер в комбо режиме или выбран айтем в дроп режиме
		// signature : void method(MyGUI::WidgetPtr _widget)
		EventSimple eventComboAccept;

	private:
		WidgetPtr mButton;
		ListPtr mList;

		bool mListShow;
		int mMaxHeight;
		size_t mItemIndex;
		bool mModeDrop;
		bool mDropMouse;
		bool mShowSmooth;

		float mDoAlpha;

	}; // class _MyGUIExport ComboBox : public Edit

} // namespace MyGUI

#endif // __MYGUI_COMBO_BOX_H__