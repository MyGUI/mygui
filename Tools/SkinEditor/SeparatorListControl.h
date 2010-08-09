/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SEPARATOR_LIST_CONTROL_H__
#define __SEPARATOR_LIST_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "SkinItem.h"
#include "PropertyAdvisor.h"

namespace tools
{

	class SeparatorListControl :
		public wraps::BaseLayout,
		public PropertyAdvisor
	{
	public:
		SeparatorListControl(MyGUI::Widget* _parent);
		virtual ~SeparatorListControl();

	private:
		void notifyChangePosition(MyGUI::List* _sender, size_t _index);

		virtual void updateSeparatorProperties();
		virtual void updateSeparatorProperty(Property* _sender, const MyGUI::UString& _value);

		void updateList();

	private:
		MyGUI::List* mList;
	};

} // namespace tools

#endif // __SEPARATOR_LIST_CONTROL_H__
