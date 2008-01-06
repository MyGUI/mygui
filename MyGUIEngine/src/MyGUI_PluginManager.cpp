/*!
	@file		PluginManager.cpp
	@author		Denis Koronchik
	@date		09/2007
	@module
*/
#include "MyGUI_PluginManager.h"
#include "MyGUI_DynLibManager.h"
#include "MyGUI_Common.h"
#include "xmlDocument.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(PluginManager);

	void PluginManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise);
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void PluginManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		unloadAllPlugins();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void PluginManager::loadPlugin(const std::string& _file)
	{
		// check initialise
		MYGUI_ASSERT(null != mIsInitialise);
		// Load plugin library
		DynLib* lib = DynLibManager::getInstance().load( _file );
		// Store for later unload
		mLibs[_file] = lib;

		// Call startup function
		DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)lib->getSymbol("dllStartPlugin");

		/*Assert(pFunc, Exception::ERR_ITEM_NOT_FOUND, "Cannot find symbol dllStartPlugin in library " + fileName,
			"PluginManager::loadPlugin");*/

		MYGUI_ASSERT(null != pFunc);// && ("Cannot find symbol dllStartPlugin in library " + fileName));

		// This must call installPlugin
		pFunc();
	}

	void PluginManager::unloadPlugin(const std::string& _file)
	{
		// check initialise
		MYGUI_ASSERT(null != mIsInitialise);

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
			MYGUI_ERROR("Plugin: " + doc.getLastError());
			return;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (root == 0) || (root->getName() != "MyGUI") ) {
			MYGUI_ERROR("Plugin: " + _file + " root tag 'MyGUI' not found");
			return;
		}

		std::string source;
		if ((false == root->findAttribute("type", source)) || (source != "plugin")) {
			MYGUI_ERROR("Skin: " + _file + " root type 'plugin' not found");
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
		MYGUI_ASSERT(null != mIsInitialise);

		_MYGUI_LOG("Installing plugin: " + _plugin->getName());

		mPlugins.insert(_plugin);
		_plugin->install();

		_plugin->initialize();
		
		_MYGUI_LOG("Plugin successfully installed");
	}

	void PluginManager::uninstallPlugin(Plugin* _plugin)
	{
		// check initialise
		MYGUI_ASSERT(null != mIsInitialise);

		_MYGUI_LOG("Uninstalling plugin: " + _plugin->getName());
		PluginList::iterator it = mPlugins.find(_plugin);
		if (it != mPlugins.end())
		{
			_plugin->shutdown();
			_plugin->uninstall();
			mPlugins.erase(it);
		}
		_MYGUI_LOG("Plugin successfully uninstalled");
	}

	void PluginManager::unloadAllPlugins()
	{
		while (false == mLibs.empty())
			unloadPlugin((*mLibs.begin()).first);
	}

} // namespace MyGUI
