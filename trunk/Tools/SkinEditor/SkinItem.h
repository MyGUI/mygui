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

namespace tools
{

	typedef std::vector<StateItem*> VectorStateItem;
	typedef MyGUI::Enumerator<VectorStateItem> EnumeratorStateItem;

	typedef MyGUI::delegates::CMultiDelegate0 EventHandle_StateChangeSelection;

	class SkinItem
	{
	public:
		SkinItem();
		~SkinItem();

		const MyGUI::UString& getName();
		void setName(const MyGUI::UString& _value);

		PropertySet* getPropertySet();

		StateItem* getChild(const MyGUI::UString& _name);

		EnumeratorStateItem getChildsEnumerator();

		StateItem* getItemSelected();
		void setItemSelected(StateItem* _value);

		EventHandle_StateChangeSelection eventChangeSelection;

	private:
		StateItem* createChild(const MyGUI::UString& _name);

		void destroyChild(StateItem* _item);

		void destroyChild(const MyGUI::UString& _name);

		void destroyAllChilds();

	private:
		MyGUI::UString mName;
		PropertySet* mPropertySet;

		VectorStateItem mChilds;
		StateItem* mItemSelected;
	};

} // namespace tools

#endif // __SKIN_ITEM_H__
