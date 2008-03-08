/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
	@module
*/
#include "MyGUI_DynLibManager.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(DynLibManager);

	void DynLibManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void DynLibManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		StringDynLibMap::iterator it;

		// unload and delete resources
		for (it = mLibsMap.begin(); it != mLibsMap.end(); it++)
		{
			it->second->unload();
			delete it->second;
		}

		// Empty the list
		mLibsMap.clear();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	DynLib* DynLibManager::load(const std::string &fileName)
	{
		StringDynLibMap::iterator it = mLibsMap.find(fileName);

		if (it != mLibsMap.end())
			return it->second;
		else
			{
				DynLib *pLib = new DynLib(fileName);
				pLib->load();
				mLibsMap[fileName] = pLib;
				return pLib;
			}

		return 0;
	}

	void DynLibManager::unload(DynLib *library)
	{
		StringDynLibMap::iterator it = mLibsMap.find(library->getName());

		if (it != mLibsMap.end())
			mLibsMap.erase(it);

		library->unload();
		delete library;
	}
}
