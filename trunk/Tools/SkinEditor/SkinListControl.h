/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_LIST_CONTROL_H__
#define __SKIN_LIST_CONTROL_H__

#include "BaseManager.h"
#include "BaseLayout/BaseLayout.h"

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

		void UpdateList();

	private:
		MyGUI::List* mList;
		MyGUI::Button* mCreate;
		MyGUI::Button* mRename;
		MyGUI::Button* mDelete;
	};

} // namespace tools

#endif // __SKIN_LIST_CONTROL_H__
