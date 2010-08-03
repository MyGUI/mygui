/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinItem.h"

namespace tools
{

	SkinItem::SkinItem()
	{
	}

	SkinItem::~SkinItem()
	{
	}

	const std::string& SkinItem::getName()
	{
		return mName;
	}

	void SkinItem::setName(const std::string& _value)
	{
		mName = _value;
	}

} // namespace tools
