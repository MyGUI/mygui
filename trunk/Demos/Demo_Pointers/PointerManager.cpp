/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/
#include "precompiled.h"
#include "PointerManager.h"

namespace demo
{

	PointerManager::PointerManager()
	{
	}

	void PointerManager::addContext(const std::string& _name)
	{
	}

	void PointerManager::removeContext(const std::string& _name)
	{
	}

	void PointerManager::setPointer(const std::string& _name)
	{
		 if (mCurrentPointer != _name)
		 {
			 mCurrentPointer = _name;
		 }
	}

} // namespace demo
