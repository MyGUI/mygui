/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _b0713d87_24ee_44cf_bee9_c96494be178e_
#define _b0713d87_24ee_44cf_bee9_c96494be178e_

#include "Control.h"
#include "sigslot.h"
#include "Data.h"
#include "TextFieldControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL ListBoxDataControl : public Control, public sigslot::has_slots<>
	{
	public:
		~ListBoxDataControl() override;

		void OnRenameData();

		void setEnableChangePosition(bool _value);

		void setDataInfo(std::string_view _parentType, std::string_view _thisType, std::string_view _propertyName);
		void setReplaceColourName(std::string_view _value);

		void addPropertyNameEnabled(std::string_view _propertyName);

		sigslot::signal2<DataPtr, DataPtr> eventChangePosition;
		sigslot::signal2<DataPtr, std::string_view> eventChangeName;

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index);
		void notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection);
		void notifyItem(MyGUI::ListBox* _sender, const MyGUI::IBNotifyItemData& _info);
		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyChangeProperty(PropertyPtr _sender);

		void invalidateList();
		void invalidateSelection();

		void connectToProperty(DataPtr _data);

		bool isDataEnabled(DataPtr _data);
		void selectListItemByData(DataPtr _data);

	private:
		MyGUI::ListBox* mListBox{nullptr};
		MyGUI::EditBox* mHelpPanel{nullptr};
		DataPtr mParentData{nullptr};
		size_t mLastIndex{MyGUI::ITEM_NONE};
		std::string mPropertyForName;
		std::string mThisType;
		MyGUI::PopupMenu* mContextMenu{nullptr};
		TextFieldControl* mTextFieldControl{nullptr};
		bool mEnableChangePosition{false};
		std::string mColourName;

		using VectorString = std::vector<std::string>;
		VectorString mPropertyNamesEnable;
	};

}

#endif
