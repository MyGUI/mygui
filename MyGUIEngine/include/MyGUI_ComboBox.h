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
		// ��� ������ ��������� ������������
		friend class factory::ComboBoxFactory;

	protected:
		ComboBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

		virtual void _onKeyButtonPressed(int _key, Char _char);

		void notifyButtonPressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyListLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyListSelectAccept(MyGUI::WidgetPtr _widget);
		void notifyListMouseItemActivate(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyEditTextChange(MyGUI::WidgetPtr _sender);

		void showList();
		void hideList();

	public:
		// ��� ������� �������
		inline static const Ogre::String & _getType() {static Ogre::String type("ComboBox"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		void setItemSelect(size_t _index);

		inline size_t getItemSelect()
		{
			return mItemIndex;
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

		size_t getItemCount();

		const Ogre::DisplayString & getItem(size_t _index);

		void setItem(size_t _index, const Ogre::DisplayString & _item);

		void insertItem(size_t _index, const Ogre::DisplayString & _item);

		void deleteItem(size_t _index);
		void deleteAllItems();

		void addItem(const Ogre::DisplayString& _item);

		inline void setSmoothShow(bool _smooth)
		{
			mShowSmooth = _smooth;
		}

		inline bool getSmoothShow()
		{
			return mShowSmooth;
		}

		// event : ����� ����� � ����� ������ ��� ������ ����� � ���� ������
		// signature : void method(MyGUI::WidgetPtr _widget)
		EventInfo_WidgetVoid eventComboAccept;

		// event : ���������� �������
		// signature : void method(MyGUI::WidgetPtr _widget, size_t _index)
		EventInfo_WidgetSizeT eventComboChangePosition;

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
