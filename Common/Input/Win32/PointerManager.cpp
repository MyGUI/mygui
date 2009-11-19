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

#include "ResourceW32Pointer.cpp"

namespace input
{

	PointerManager::PointerManager() :
		mHwnd(0),
		mManagerPointer(true)
	{
	}

	PointerManager::~PointerManager()
	{
	}

	void PointerManager::createPointerManager(size_t _handle)
	{
		mHwnd = _handle;

		MyGUI::PointerManager& manager = MyGUI::PointerManager::getInstance();
		manager.setVisible(false);
		manager.eventChangeMousePointer += MyGUI::newDelegate(this, &PointerManager::notifyChangeMousePointer);

		MyGUI::FactoryManager::getInstance().registryFactory<ResourceW32Pointer>("Resource");
		MyGUI::Gui::getInstance().load("core_pointers_W32.xml");
	}

	void PointerManager::destroyPointerManager()
	{
		MyGUI::FactoryManager::getInstance().unregistryFactory<ResourceW32Pointer>("Resource");

		MyGUI::PointerManager& manager = MyGUI::PointerManager::getInstance();
		manager.eventChangeMousePointer -= MyGUI::newDelegate(this, &PointerManager::notifyChangeMousePointer);
	}

	void PointerManager::setPointerVisible(bool _value)
	{
		ShowCursor(_value);
	}

	void PointerManager::notifyChangeMousePointer(const std::string& _name)
	{
		if (mManagerPointer)
		{
			setPointer(_name);
		}
	}

	void PointerManager::setPointerHandle(size_t _id)
	{
		SetClassLong((HWND)mHwnd, GCL_HCURSOR, (LONG)_id);
		if ((GetCapture() == (HWND)mHwnd)
			|| isMouseInClient())
		{
			::SetCursor((HCURSOR)_id);
		}
	}

	bool PointerManager::isMouseInClient()
	{
		POINT point = { 0, 0 };
		ClientToScreen((HWND)mHwnd, &point);
		// x и y всегда 0
		RECT client_rect = { 0, 0, 0, 0 };
		GetClientRect((HWND)mHwnd, &client_rect);

		POINT cursor_point = { 0, 0 };
		GetCursorPos(&cursor_point);

		bool hor = cursor_point.x >= point.x && cursor_point.x < (point.x + client_rect.right);
		bool ver = cursor_point.y >= point.y && cursor_point.y < (point.y + client_rect.bottom);

		return hor && ver;
	}

	void PointerManager::setPointerName(const std::string& _name)
	{
		mManagerPointer = false;
		setPointer(_name);
	}

	void PointerManager::setPointer(const std::string& _name)
	{
		MapPointer::iterator iter = mMapGuiPointer.find(_name);
		if (iter != mMapGuiPointer.end())
		{
			setPointerHandle(iter->second);
		}
		else
		{
			MyGUI::IResource* resource_generic = MyGUI::ResourceManager::getInstance().getByName(_name, false);
			if (resource_generic != nullptr)
			{
				ResourceW32Pointer* resource = resource_generic->castType<ResourceW32Pointer>(false);
				if (resource != nullptr)
				{
					mMapGuiPointer[_name] = resource->getPointerHandle();
					setPointerHandle(resource->getPointerHandle());
				}
			}
		}
	}

} // namespace input
