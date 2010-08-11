/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __REGION_LIST_CONTROL_H__
#define __REGION_LIST_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "RegionItem.h"
#include "PropertyAdvisor.h"

namespace tools
{

	class RegionListControl :
		public wraps::BaseLayout,
		public PropertyAdvisor
	{
	public:
		RegionListControl(MyGUI::Widget* _parent);
		virtual ~RegionListControl();

	private:
		void notifyChangePosition(MyGUI::List* _sender, size_t _index);

		virtual void updateRegionProperties();
		virtual void updateSeparatorProperties();

		virtual void updateRegionProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner);

		void updateList();
		void updateRegionEnabled();

		bool isSeparatorVisible(MyGUI::Align _value);

	private:
		MyGUI::List* mList;
		MyGUI::UString mTypeName;
	};

} // namespace tools

#endif // __REGION_LIST_CONTROL_H__
