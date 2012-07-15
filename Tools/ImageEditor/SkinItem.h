/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _ab3415aa_44d0_431f_afea_9960cdb57f30_
#define _ab3415aa_44d0_431f_afea_9960cdb57f30_

#include <MyGUI.h>
#include "PropertySet.h"
#include "StateItem.h"
#include "SeparatorItem.h"
#include "RegionItem.h"
#include "ItemHolder.h"

/*namespace tools
{

	class SkinItem :
		public MyGUI::ISerializable
	{
	public:
		SkinItem();
		~SkinItem();

		const MyGUI::UString& getName() const;
		void setName(const MyGUI::UString& _value);

		PropertySet* getPropertySet();

		ItemHolder<StateItem>& getStates();
		ItemHolder<SeparatorItem>& getSeparators();
		ItemHolder<RegionItem>& getRegions();

		bool getUniqum() const;
		void setUniqum(bool _value);

		virtual void serialization(MyGUI::xml::Element* _node, MyGUI::Version _version);

		void deserialization2(MyGUI::xml::Element* _node, MyGUI::Version _version);

	private:
		void parseProperty(MyGUI::xml::Element* _node);
		void parseRegions(MyGUI::xml::Element* _node);
		void parseSeparators(MyGUI::xml::Element* _node);
		void parseStates(MyGUI::xml::Element* _node);

		MyGUI::IntPoint getNormalStateOffset(MyGUI::xml::Element* _node);

	private:
		MyGUI::UString mName;
		PropertySet* mPropertySet;

		ItemHolder<StateItem> mStates;
		ItemHolder<SeparatorItem> mSeparators;
		ItemHolder<RegionItem> mRegions;
		bool mUniqum;
	};

}*/

#endif
