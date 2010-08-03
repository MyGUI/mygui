/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_LIST_CONTROL_H__
#define __SKIN_LIST_CONTROL_H__

#include "BaseManager.h"
#include "BaseLayout/BaseLayout.h"
#include "TextFieldControl.h"
#include "SkinItem.h"

namespace tools
{

	class SkinListControl :
		public wraps::BaseLayout
	{
	public:
		SkinListControl(MyGUI::Widget* _parent);
		virtual ~SkinListControl();

	private:
		void notifyChangePosition(MyGUI::List* _sender, size_t _index);
		void notifyCreate(MyGUI::Widget* _sender);
		void notifyRename(MyGUI::Widget* _sender);
		void notifyDelete(MyGUI::Widget* _sender);
		void notifyTextFieldResult(bool _result);

		void updateList();

		void hideTextField();
		void showTextField(SkinItem* _item);

		MyGUI::UString getNextFreeName();

		void renameItem(SkinItem* _item, const MyGUI::UString& _value);
		void createItem(const MyGUI::UString& _value);

	private:
		MyGUI::List* mList;
		MyGUI::Button* mCreate;
		MyGUI::Button* mRename;
		MyGUI::Button* mDelete;

		TextFieldControl* mTextFieldControl;
	};

} // namespace tools

#endif // __SKIN_LIST_CONTROL_H__
