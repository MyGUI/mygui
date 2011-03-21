/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __REGION_ITEM_H__
#define __REGION_ITEM_H__

#include <MyGUI.h>
#include "PropertySet.h"

namespace tools
{

	class RegionItem :
		public MyGUI::ISerializable
	{
	public:
		RegionItem();
		~RegionItem();

		const MyGUI::UString& getName() const;
		void setName(const MyGUI::UString& _value);

		PropertySet* getPropertySet();

		MyGUI::Align getSeparator();
		void setSeparator(MyGUI::Align _value);

		virtual void serialization(MyGUI::xml::Element* _node, MyGUI::Version _version);
		//virtual void deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version);

	private:
		MyGUI::UString mName;
		PropertySet* mPropertySet;
	};

} // namespace tools

#endif // __REGION_ITEM_H__
