/*!
	@file		PluginManager.cpp
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/
#include "MyGUI_PluginManager.h"
#include "MyGUI_DynLibManager.h"
#include "MyGUI_LogManager.h"
#include "xmlDocument.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(PluginManager);

	void PluginManager::initialise()
	{
		assert(!mIsInitialise);

		LogManager::getInstance().out("* Initialize: Plugin manager");

		LogManager::getInstance().out("Manager successfully initialized");

		mIsInitialise = true;
	}

	void PluginManager::shutdown()
	{
		if (!mIsInitialise) return;

		LogManager::getInstance().out("* Shut down: Plugin manager");
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
		mLibs[_file] = lib;

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

		DynLibList::iterator it = mLibs.find(_file);
		if (it != mLibs.end()) {
			// Call plugin shutdown
			DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*it).second->getSymbol("dllStopPlugin");
			// this must call uninstallPlugin
			pFunc();
			// Unload library (destroyed by DynLibManager)
			DynLibManager::getInstance().unload((*it).second);
			mLibs.erase(it);
		}
	}

	void PluginManager::loadPluginCfg(const std::string& _file)
	{
		xml::xmlDocument doc;
		if (false == doc.open(_file)) {
			LogManager::getInstance().out("ERROR : ", doc.getLastError(), " | void PluginManager::loadPluginCfg(const std::string& _file)");
			return;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (root == 0) || (root->getName() != "MyGUI") ) {
			LogManager::getInstance().out("ERROR : not find root tag 'MyGUI'", " | void PluginManager::loadPluginCfg(const std::string& _file)");
			return;
		}

		std::string source;
		if ((false == root->findAttribute("type", source)) || (source != "plugin")) {
			LogManager::getInstance().out("ERROR : not find root type 'plugin'", " | void PluginManager::loadPluginCfg(const std::string& _file)");
			return;
		}

		xml::xmlNodeIterator node = root->getNodeIterator();
		while (node.nextNode("path")) {
			if (node->findAttribute("source", source)) loadPlugin(source);
		}
	}

	void PluginManager::installPlugin(Plugin* _plugin)
	{
		// check initialise
		assert(mIsInitialise);

		LogManager::getInstance().out("Installing plugin: " + _plugin->getName());

		mPlugins.insert(_plugin);
		_plugin->install();

		_plugin->initialize();
		
		LogManager::getInstance().out("Plugin successfully installed");
	}

	void PluginManager::uninstallPlugin(Plugin* _plugin)
	{
		// check initialise
		assert(mIsInitialise);

		LogManager::getInstance().out("Uninstalling plugin: " + _plugin->getName());
		PluginList::iterator it = mPlugins.find(_plugin);
		if (it != mPlugins.end())
		{
			_plugin->shutdown();
			_plugin->uninstall();
			mPlugins.erase(it);
		}
		LogManager::getInstance().out("Plugin successfully uninstalled");
	}

	void PluginManager::unloadAllPlugins()
	{
		while (false == mLibs.empty())
			unloadPlugin((*mLibs.begin()).first);
	}

} // namespace MyGUI