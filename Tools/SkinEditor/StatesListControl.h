/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATES_LIST_CONTROL_H__
#define __STATES_LIST_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "SkinItem.h"

namespace tools
{

	class StatesListControl :
		public wraps::BaseLayout
	{
	public:
		StatesListControl(MyGUI::Widget* _parent);
		virtual ~StatesListControl();

	private:
		void notifyChangePosition(MyGUI::List* _sender, size_t _index);
		void notifyChangeSelection();
		void notifyChangeProperty(Property* _sender, const MyGUI::UString& _value);

		void advice();
		void unadvice();

		void updateList();

		bool isValueTrue(const MyGUI::UString& _value);

	private:
		MyGUI::List* mList;

		SkinItem* mCurrentSkin;
	};

} // namespace tools

#endif // __STATES_LIST_CONTROL_H__
