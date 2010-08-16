/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __OPTION_PATH_CONTROL_H__
#define __OPTION_PATH_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
//#include "TextFieldControl.h"
//#include "SkinItem.h"

namespace tools
{

	class OptionPathControl :
		public wraps::BaseLayout
	{
	public:
		OptionPathControl(MyGUI::Widget* _parent);
		virtual ~OptionPathControl();

	/*private:
		void notifyChangePosition(MyGUI::List* _sender, size_t _index);
		void notifyCreate(MyGUI::Widget* _sender);
		void notifyRename(MyGUI::Widget* _sender);
		void notifyDelete(MyGUI::Widget* _sender);
		void notifyTextFieldResult(bool _result);
		void notifyDeleteMessageBoxResult(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _style);
		void notifyChangeList();

		void updateList();

		void hideTextField();
		void showTextField(SkinItem* _item);

		MyGUI::UString getNextFreeName();
		bool isNameExist(const MyGUI::UString& _value);
		size_t getNameCount(const MyGUI::UString& _value);

		void renameItem(SkinItem* _item, const MyGUI::UString& _value);
		void createItem(const MyGUI::UString& _value);

	private:
		MyGUI::List* mList;
		MyGUI::Button* mCreate;
		MyGUI::Button* mRename;
		MyGUI::Button* mDelete;

		TextFieldControl* mTextFieldControl;*/
	};

} // namespace tools

#endif // __OPTION_PATH_CONTROL_H__
