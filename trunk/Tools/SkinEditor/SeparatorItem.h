/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SEPARATOR_ITEM_H__
#define __SEPARATOR_ITEM_H__

#include <MyGUI.h>
#include "PropertySet.h"

namespace tools
{

	class SeparatorItem :
		public MyGUI::ISerializable
	{
	public:
		SeparatorItem();
		~SeparatorItem();

		const MyGUI::UString& getName();
		void setName(const MyGUI::UString& _value);

		MyGUI::Align getCorner();
		void setCorner(MyGUI::Align _value);

		PropertySet* getPropertySet();

		virtual void serialization(MyGUI::xml::Element* _node, MyGUI::Version _version);
		virtual void deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version);

	private:
		MyGUI::UString mName;
		PropertySet* mPropertySet;
	};

} // namespace tools

#endif // __SEPARATOR_ITEM_H__
