/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_ITEM_H__
#define __SKIN_ITEM_H__

#include "BaseManager.h"

namespace tools
{

	class SkinItem
	{
	public:
		SkinItem();
		~SkinItem();

		const MyGUI::UString& getName();
		void setName(const MyGUI::UString& _value);

	private:
		MyGUI::UString mName;
	};

} // namespace tools

#endif // __SKIN_ITEM_H__
