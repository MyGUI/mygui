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

	class SeparatorItem
	{
	public:
		SeparatorItem();
		~SeparatorItem();

		const MyGUI::UString& getName();
		void setName(const MyGUI::UString& _value);

		bool getHorizontal();
		void setHorizontal(bool _value);


		PropertySet* getPropertySet();

	private:
		MyGUI::UString mName;
		PropertySet* mPropertySet;
		bool mHorizontal;
	};

} // namespace tools

#endif // __SEPARATOR_ITEM_H__
