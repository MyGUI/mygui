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

		const std::string& getName();
		void setName(const std::string& _value);

	private:
		std::string mName;
	};

} // namespace tools

#endif // __SKIN_ITEM_H__
