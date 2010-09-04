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
#include "RegionItem.h"
#include "ItemHolder.h"

namespace tools
{

	class SkinItem :
		public MyGUI::ISerializable
	{
	public:
		SkinItem();
		~SkinItem();

		const MyGUI::UString& getName();
		void setName(const MyGUI::UString& _value);

		PropertySet* getPropertySet();

		ItemHolder<StateItem>& getStates() { return mStates; }
		ItemHolder<SeparatorItem>& getSeparators() { return mSeparators; }
		ItemHolder<RegionItem>& getRegions() { return mRegions; }

		virtual void serialization(MyGUI::xml::Element* _node, MyGUI::Version _version);
		virtual void deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version);

	private:
		MyGUI::UString mName;
		PropertySet* mPropertySet;

		ItemHolder<StateItem> mStates;
		ItemHolder<SeparatorItem> mSeparators;
		ItemHolder<RegionItem> mRegions;
	};

} // namespace tools

#endif // __SKIN_ITEM_H__
