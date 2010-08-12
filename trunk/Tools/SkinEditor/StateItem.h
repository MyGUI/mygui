/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATE_ITEM_H__
#define __STATE_ITEM_H__

#include <MyGUI.h>
#include "PropertySet.h"

namespace tools
{

	class StateItem :
		public MyGUI::ISerializable
	{
	public:
		StateItem();
		~StateItem();

		const MyGUI::UString& getName();
		void setName(const MyGUI::UString& _value);

		PropertySet* getPropertySet();

		virtual void serialization(MyGUI::xml::Element* _node, MyGUI::Version _version);
		virtual void deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version);

	private:
		MyGUI::UString mName;
		PropertySet* mPropertySet;
	};

} // namespace tools

#endif // __STATE_ITEM_H__
