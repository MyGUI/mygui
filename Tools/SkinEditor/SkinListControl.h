/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_LIST_CONTROL_H__
#define __SKIN_LIST_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "TextFieldControl.h"
#include "SkinItem.h"
#include "MessageBox/MessageBox.h"

namespace tools
{
	class SkinListControl :
		public wraps::BaseLayout
	{
	public:
		SkinListControl(MyGUI::Widget* _parent);
		virtual ~SkinListControl();

	private:
		void notifyChangePosition(MyGUI::ListBox* _sender, size_t _index);
		void notifyCreate(MyGUI::Widget* _sender);
		void notifyRename(MyGUI::Widget* _sender);
		void notifyDelete(MyGUI::Widget* _sender);
		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyDeleteMessageBoxResult(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _style);
		void notifyChangeList();

		void updateList();

		void showTextField(SkinItem* _item);

		MyGUI::UString getNextFreeName();
		bool isNameExist(const MyGUI::UString& _value);
		size_t getNameCount(const MyGUI::UString& _value);

		void renameItem(SkinItem* _item, const MyGUI::UString& _value);
		void createItem(const MyGUI::UString& _value);

		std::string getLastTextureName();

	private:
		MyGUI::ListBox* mList;
		MyGUI::Button* mCreate;
		MyGUI::Button* mRename;
		MyGUI::Button* mDelete;

		TextFieldControl* mTextFieldControl;
	};

} // namespace tools

#endif // __SKIN_LIST_CONTROL_H__
