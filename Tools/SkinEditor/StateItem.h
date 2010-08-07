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

	class StateItem
	{
	public:
		StateItem();
		~StateItem();

		const MyGUI::UString& getName();
		void setName(const MyGUI::UString& _value);

		PropertySet* getPropertySet();

	private:
		MyGUI::UString mName;
		PropertySet* mPropertySet;
	};

} // namespace tools

#endif // __STATE_ITEM_H__
