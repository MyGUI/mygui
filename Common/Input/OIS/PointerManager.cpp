/*!
	@file
	@author		Albert Semenov
	@date		1/2009
*/

#include "Precompiled.h"
#include "PointerManager.h"
#include <MyGUI.h>

namespace input
{

	PointerManager::PointerManager()
	{
	}

	PointerManager::~PointerManager()
	{
	}

	void PointerManager::createPointerManager(size_t _handle)
	{
	}

	void PointerManager::destroyPointerManager()
	{
	}

	void PointerManager::setPointerVisible(bool _value)
	{
		MyGUI::PointerManager::getInstance().setVisible(_value);
	}

	void PointerManager::setPointerName(const std::string& _name)
	{
		MyGUI::PointerManager::getInstance().setPointer(_name);
	}

	void PointerManager::loadPointerResources()
	{
	}

} // namespace input
