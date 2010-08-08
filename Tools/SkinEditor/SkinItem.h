/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_ITEM_H__
#define __SKIN_ITEM_H__

#include <MyGUI.h>
#include "PropertySet.h"
#include "StateItem.h"
#include "SeparatorItem.h"
#include "ItemHolder.h"

namespace tools
{

	class SkinItem
	{
	public:
		SkinItem();
		~SkinItem();

		const MyGUI::UString& getName();
		void setName(const MyGUI::UString& _value);

		PropertySet* getPropertySet();

		ItemHolder<StateItem>& getStates() { return mStates; }

	private:
		MyGUI::UString mName;
		PropertySet* mPropertySet;

		ItemHolder<StateItem> mStates;
	};

} // namespace tools

#endif // __SKIN_ITEM_H__
