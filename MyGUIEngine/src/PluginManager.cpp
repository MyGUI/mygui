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


	PluginManager* PluginManager::m_instance = 0;

	PluginManager::PluginManager()
	{

	}

	PluginManager::~PluginManager()
	{
		LOG_MESSAGE("* Shut down: Plugin manager");

		unloadAllPlugins();
	}

	PluginManager* PluginManager::Instance()
	{
		if (!m_instance)
			m_instance = new PluginManager();

		return m_instance;
	}

	void PluginManager::shutDown()
	{
		if (m_instance)
			delete m_instance;

		m_instance = 0;
	}

	void PluginManager::initialize()
	{
		LOG_MESSAGE("* Initialize: Plugin manager");

		LOG_MESSAGE("Manager successfully initialized");
	}

	void PluginManager::loadPlugin(const std::string &fileName)
	{
		return; // ???
		// Load plugin library
		DynLib* lib = DynLibManager::Instance()->load( fileName );
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

	void PluginManager::unloadPlugin(const std::string &fileName)
	{
		DynLibList::iterator it;

		for (it = mLibs.begin(); it != mLibs.end(); ++it)
		{
			if ((*it)->getName() == fileName)
			{
				// Call plugin shutdown
				DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*it)->getSymbol("dllStopPlugin");
				// this must call uninstallPlugin
				pFunc();
				// Unload library (destroyed by DynLibManager)
				DynLibManager::Instance()->unload(*it);
				mLibs.erase(it);

				return;
			}

		}
	}

	void PluginManager::installPlugin(Plugin *plugin)
	{
		LOG_MESSAGE("Installing plugin: " + plugin->getName());

		mPlugins.push_back(plugin);
		plugin->install();

		plugin->initialize();
		
		LOG_MESSAGE("Plugin successfully installed");
	}

	void PluginManager::uninstallPlugin(Plugin *plugin)
	{
		LOG_MESSAGE("Uninstalling plugin: " + plugin->getName());
		PluginList::iterator it = std::find(mPlugins.begin(), mPlugins.end(), plugin);
		if (it != mPlugins.end())
		{
			plugin->shutDown();
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

}