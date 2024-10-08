/*!
	@file
	@author		Albert Semenov
	@date		1/2009
*/

#include "Precompiled.h"
#include "PointerManager.h"
#include <MyGUI.h>

#include "ResourceSDLPointer.h"

namespace input
{

	PointerManager::~PointerManager()
	{
		SDL_FreeCursor(mCursor);
	}

	void PointerManager::createPointerManager()
	{
		MyGUI::PointerManager& manager = MyGUI::PointerManager::getInstance();
		manager.setVisible(false);
		manager.eventChangeMousePointer += MyGUI::newDelegate(this, &PointerManager::notifyChangeMousePointer);

		const std::string& resourceCategory = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<ResourceSDLPointer>(resourceCategory);
	}

	void PointerManager::destroyPointerManager()
	{
		const std::string& resourceCategory = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<ResourceSDLPointer>(resourceCategory);

		MyGUI::PointerManager& manager = MyGUI::PointerManager::getInstance();
		manager.eventChangeMousePointer -= MyGUI::newDelegate(this, &PointerManager::notifyChangeMousePointer);
	}

	void PointerManager::setPointerVisible(bool _value)
	{
		SDL_ShowCursor(static_cast<int>(_value));
	}

	void PointerManager::notifyChangeMousePointer(std::string_view _name)
	{
		if (mManagerPointer)
		{
			setPointer(_name);
		}
	}

	void PointerManager::setPointerName(std::string_view _name)
	{
		mManagerPointer = false;
		setPointer(_name);
	}

	void PointerManager::updateSDLPointer(SDL_SystemCursor _newCursor)
	{
		SDL_FreeCursor(mCursor);
		mCursor = SDL_CreateSystemCursor(_newCursor);
		SDL_SetCursor(mCursor);
	}

	void PointerManager::setPointer(std::string_view _name)
	{
		MapPointer::iterator iter = mMapPointer.find(_name);
		if (iter != mMapPointer.end())
		{
			updateSDLPointer(iter->second);
		}
		else
		{
			MyGUI::IResource* resource_generic = MyGUI::ResourceManager::getInstance().getByName(_name, false);
			if (resource_generic != nullptr)
			{
				ResourceSDLPointer* resource = resource_generic->castType<ResourceSDLPointer>(false);
				if (resource != nullptr)
				{
					mMapPointer.emplace(_name, resource->getPointerType());
					updateSDLPointer(resource->getPointerType());
				}
			}
		}
	}

	void PointerManager::loadPointerResources()
	{
		MyGUI::ResourceManager::getInstance().load("PointersSDL.xml");
	}

} // namespace input
