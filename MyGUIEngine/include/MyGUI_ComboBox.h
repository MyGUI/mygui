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
		// ��� ������ ��������� ������������
		friend factory::ComboBoxFactory;

	protected:
		ComboBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

		virtual void _onKeyButtonPressed(int _key, wchar_t _char);

		void notifyButtonPressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyListLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyListSelectAccept(MyGUI::WidgetPtr _widget);
		void notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyListMouseChangePosition(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyEditTextChange(MyGUI::WidgetPtr _sender);

		void showList();
		void hideList();

	public:
		// ��� ������� �������
		inline static const Ogre::String & getType() {static Ogre::String type("ComboBox"); return type;}

		inline void setComboItemIndex(size_t _index)
		{
			MYGUI_ASSERT(_index < mList->getItemCount());
			mItemIndex = _index;
			setCaption(mList->getItemString(_index));
		}

		inline size_t getComboItemIndex()
		{
			return mItemIndex;
		}

		inline void setComboText(const Ogre::DisplayString& _text)
		{
			if (false == mModeDrop) setCaption(_text);
		}

		inline const Ogre::DisplayString& getComboText()
		{
			return getCaption();
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

		// event : ����� ����� � ����� ������ ��� ������ ����� � ���� ������
		// signature : void method(MyGUI::WidgetPtr _widget)
		EventSimple eventComboAccept;

	private:
		WidgetPtr mButton;
		ListPtr mList;

		bool mListShow;
		int mMaxHeight;
		size_t mItemIndex;
		bool mModeDrop;

	}; // class _MyGUIExport StaticText : public Widget

} // namespace MyGUI

#endif // __MYGUI_COMBO_BOX_H__