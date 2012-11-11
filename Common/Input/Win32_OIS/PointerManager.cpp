/*!
	@file
	@author		Albert Semenov
	@date		1/2009
	@module
*/

#include "Precompiled.h"
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

		std::string resourceCategory = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<ResourceW32Pointer>(resourceCategory);
	}

	void PointerManager::destroyPointerManager()
	{
		std::string resourceCategory = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<ResourceW32Pointer>(resourceCategory);

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
		SetClassLongPtr((HWND)mHwnd, GCLP_HCURSOR, (LONG_PTR)_id);
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

	void PointerManager::loadPointerResources()
	{
		MyGUI::ResourceManager::getInstance().load("PointersW32.xml");
	}

} // namespace input
