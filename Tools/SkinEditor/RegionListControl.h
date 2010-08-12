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
		virtual void updateSkinProperties();

		virtual void updateRegionProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);

		void updateList();

		void updateCoord();

		void updateRegionEnabled();
		void updateRegionPosition();

		bool isSeparatorVisible(MyGUI::Align _value);
		int getSeparatorPosition(MyGUI::Align _value);

		RegionItem* getRegion(MyGUI::Align _align);

	private:
		MyGUI::List* mList;
		MyGUI::UString mTypeName;

		MyGUI::IntCoord mCoordValue;
	};

} // namespace tools

#endif // __REGION_LIST_CONTROL_H__
