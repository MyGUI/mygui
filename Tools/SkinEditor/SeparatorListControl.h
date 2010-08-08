/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SEPARATOR_LIST_CONTROL_H__
#define __SEPARATOR_LIST_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "SkinItem.h"

namespace tools
{

	class SeparatorListControl :
		public wraps::BaseLayout
	{
	public:
		SeparatorListControl(MyGUI::Widget* _parent);
		virtual ~SeparatorListControl();

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

#endif // __SEPARATOR_LIST_CONTROL_H__
