/*!
	@file
	@author		Albert Semenov
	@date		1/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "precompiled.h"
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

} // namespace input
