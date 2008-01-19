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
		// тип данного виджета
		inline static const Ogre::String & _getType() {static Ogre::String type("ComboBox"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		void setComboItemIndex(size_t _index);

		inline size_t getComboItemIndex()
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

		const Ogre::DisplayString & getItemString(size_t _index);

		void setItemString(size_t _index, const Ogre::DisplayString & _item);

		void insertItemString(size_t _index, const Ogre::DisplayString & _item);

		void deleteItemString(size_t _index);

		void addItemString(const Ogre::DisplayString& _item);

		inline void setSmoothShow(bool _smooth)
		{
			mShowSmooth = _smooth;
		}

		inline bool getSmoothShow()
		{
			return mShowSmooth;
		}

		// event : нажат энтер в комбо режиме или выбран айтем в дроп режиме
		// signature : void method(MyGUI::WidgetPtr _widget)
		EventInfo_WidgetVoid eventComboAccept;

		// event : изменилась позиция
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
