/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_DynLibManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	template <> DynLibManager* Singleton<DynLibManager>::msInstance = nullptr;
	template <> const char* Singleton<DynLibManager>::mClassTypeName = "DynLibManager";

	DynLibManager::DynLibManager() :
		mIsInitialise(false)
	{
	}

	void DynLibManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		Gui::getInstance().eventFrameStart += newDelegate(this, &DynLibManager::notifyEventFrameStart);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void DynLibManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		unloadAll();

		Gui::getInstance().eventFrameStart -= newDelegate(this, &DynLibManager::notifyEventFrameStart);
		_unloadDelayDynLibs();

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	DynLib* DynLibManager::load(const std::string& fileName)
	{
		StringDynLibMap::iterator it = mLibsMap.find(fileName);

		if (it != mLibsMap.end())
		{
			return it->second;
		}

		DynLib* pLib = new DynLib(fileName);
		if (!pLib->load())
		{
			delete pLib;
			return 0;
		}

		mLibsMap[fileName] = pLib;
		return pLib;
	}

	void DynLibManager::unload(DynLib* library)
	{
		StringDynLibMap::iterator it = mLibsMap.find(library->getName());

		if (it != mLibsMap.end())
			mLibsMap.erase(it);

		mDelayDynLib.push_back(library);
	}

	void DynLibManager::unloadAll()
	{
		// unload and delete resources
		for (StringDynLibMap::iterator it = mLibsMap.begin(); it != mLibsMap.end(); ++it)
		{
			mDelayDynLib.push_back(it->second);
		}
		// Empty the list
		mLibsMap.clear();
	}

	void DynLibManager::notifyEventFrameStart(float _time)
	{
		_unloadDelayDynLibs();
	}

	void DynLibManager::_unloadDelayDynLibs()
	{
		if (!mDelayDynLib.empty())
		{
			WidgetManager* manager = WidgetManager::getInstancePtr();
			if (manager != nullptr)
				manager->_deleteDelayWidgets();

			for (VectorDynLib::iterator entry = mDelayDynLib.begin(); entry != mDelayDynLib.end(); ++entry)
			{
				(*entry)->unload();
				delete (*entry);
			}
			mDelayDynLib.clear();
		}
	}

} // namespace MyGUI
