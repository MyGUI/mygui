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

		StateItem* getState(const MyGUI::UString& _name);

		EnumeratorStateItem getStatesEnumerator();

		StateItem* getStateSelected();
		void setStateSelected(StateItem* _value);

		EventHandle_StateChangeSelection eventStateChangeSelection;

	private:
		StateItem* createState(const MyGUI::UString& _name);

		void destroyState(StateItem* _item);

		void destroyState(const MyGUI::UString& _name);

		void destroyAllStates();

	private:
		MyGUI::UString mName;
		PropertySet* mPropertySet;

		VectorStateItem mStates;
		StateItem* mStateSelected;
	};

} // namespace tools

#endif // __SKIN_ITEM_H__
