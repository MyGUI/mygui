/*!
	@file		PluginManager.cpp
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/
#include "PluginManager.h"
#include "DynLibManager.h"
#include <algorithm>

namespace MyGUI
{

	INSTANCE_IMPLEMENT(PluginManager);

	void PluginManager::initialise()
	{
		assert(!mIsInitialise);

		LOG_MESSAGE("* Initialize: Plugin manager");

		LOG_MESSAGE("Manager successfully initialized");

		mIsInitialise = true;
	}

	void PluginManager::shutdown()
	{
		if (!mIsInitialise) return;

		LOG_MESSAGE("* Shut down: Plugin manager");
		unloadAllPlugins();

		mIsInitialise = false;
	}

	void PluginManager::loadPlugin(const std::string& _file)
	{
		// check initialise
		assert(mIsInitialise);
		// Load plugin library
		DynLib* lib = DynLibManager::getInstance().load( _file );
		// Store for later unload
		mLibs.push_back(lib);

		// Call startup function
		DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)lib->getSymbol("dllStartPlugin");

		/*Assert(pFunc, Exception::ERR_ITEM_NOT_FOUND, "Cannot find symbol dllStartPlugin in library " + fileName,
			"PluginManager::loadPlugin");*/

		assert(pFunc);// && ("Cannot find symbol dllStartPlugin in library " + fileName));

		// This must call installPlugin
		pFunc();
	}

	void PluginManager::unloadPlugin(const std::string& _file)
	{
		// check initialise
		assert(mIsInitialise);

		for (DynLibList::iterator it = mLibs.begin(); it != mLibs.end(); ++it) {
			if ((*it)->getName() == _file) {
				// Call plugin shutdown
				DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*it)->getSymbol("dllStopPlugin");
				// this must call uninstallPlugin
				pFunc();
				// Unload library (destroyed by DynLibManager)
				DynLibManager::getInstance().unload(*it);
				mLibs.erase(it);

				return;
			}

		}
	}

	void PluginManager::installPlugin(Plugin *plugin)
	{
		// check initialise
		assert(mIsInitialise);

		LOG_MESSAGE("Installing plugin: " + plugin->getName());

		mPlugins.push_back(plugin);
		plugin->install();

		plugin->initialize();
		
		LOG_MESSAGE("Plugin successfully installed");
	}

	void PluginManager::uninstallPlugin(Plugin *plugin)
	{
		// check initialise
		assert(mIsInitialise);

		// unload plugin before uninstall
		unloadPlugin(plugin->getName());

		LOG_MESSAGE("Uninstalling plugin: " + plugin->getName());
		PluginList::iterator it = std::find(mPlugins.begin(), mPlugins.end(), plugin);
		if (it != mPlugins.end())
		{
			plugin->shutdown();
			plugin->uninstall();
			mPlugins.erase(it);
		}
		LOG_MESSAGE("Plugin successfully uninstalled");
	}

	void PluginManager::unloadAllPlugins()
	{
		while (!mPlugins.empty())
			uninstallPlugin(*mPlugins.begin());
	}

} // namespace MyGUI