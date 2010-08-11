/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATES_LIST_CONTROL_H__
#define __STATES_LIST_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "SkinItem.h"
#include "PropertyAdvisor.h"

namespace tools
{

	class StatesListControl :
		public wraps::BaseLayout,
		public PropertyAdvisor
	{
	public:
		StatesListControl(MyGUI::Widget* _parent);
		virtual ~StatesListControl();

	private:
		void notifyChangePosition(MyGUI::List* _sender, size_t _index);

		virtual void updateStateProperties();
		virtual void updateStateProperty(Property* _sender, const MyGUI::UString& _owner);

		void updateList();

	private:
		MyGUI::List* mList;
	};

} // namespace tools

#endif // __STATES_LIST_CONTROL_H__
