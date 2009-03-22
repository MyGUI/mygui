/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
	@module
*//*
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_PluginManager.h"
#include "MyGUI_DynLibManager.h"
#include "MyGUI_Common.h"

namespace MyGUI
{
	const std::string XML_TYPE("Plugin");

	MYGUI_INSTANCE_IMPLEMENT(PluginManager);

	void PluginManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &PluginManager::_load);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void PluginManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		unloadAllPlugins();
		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void PluginManager::loadPlugin(const std::string& _file)
	{
		// check initialise
		MYGUI_ASSERT(mIsInitialise, INSTANCE_TYPE_NAME << "used but not initialised");
		// Load plugin library
		DynLib* lib = DynLibManager::getInstance().load( _file );
		// Store for later unload
		mLibs[_file] = lib;

		// Call startup function
		DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)lib->getSymbol("dllStartPlugin");

		/*Assert(pFunc, Exception::ERR_ITEM_NOT_FOUND, "Cannot find symbol dllStartPlugin in library " + fileName,
			"PluginManager::loadPlugin");*/

		MYGUI_ASSERT(nullptr != pFunc, INSTANCE_TYPE_NAME << "Cannot find symbol 'dllStartPlugin' in library " << _file);

		// This must call installPlugin
		pFunc();
	}

	void PluginManager::unloadPlugin(const std::string& _file)
	{
		// check initialise
		MYGUI_ASSERT(mIsInitialise, INSTANCE_TYPE_NAME << "used but not initialised");

		DynLibList::iterator it = mLibs.find(_file);
		if (it != mLibs.end()) {
			// Call plugin shutdown
			DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*it).second->getSymbol("dllStopPlugin");

			MYGUI_ASSERT(nullptr != pFunc, INSTANCE_TYPE_NAME << "Cannot find symbol 'dllStopPlugin' in library " << _file);

			// this must call uninstallPlugin
			pFunc();
			// Unload library (destroyed by DynLibManager)
			DynLibManager::getInstance().unload((*it).second);
			mLibs.erase(it);
		}
	}

	bool PluginManager::load(const std::string& _file, const std::string & _group)
	{
		return ResourceManager::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void PluginManager::_load(xml::ElementPtr _node, const std::string & _file, Version _version)
	{
		xml::ElementEnumerator node = _node->getElementEnumerator();
		std::string source;
		while (node.next("path")) {
			if (node->findAttribute("source", source)) loadPlugin(source);
		}
	}

	void PluginManager::installPlugin(IPlugin* _plugin)
	{
		// check initialise
		MYGUI_ASSERT(mIsInitialise, INSTANCE_TYPE_NAME << "used but not initialised");

		MYGUI_LOG(Info, "Installing plugin: " << _plugin->getName());

		mPlugins.insert(_plugin);
		_plugin->install();

		_plugin->initialize();

		MYGUI_LOG(Info, "Plugin successfully installed");
	}

	void PluginManager::uninstallPlugin(IPlugin* _plugin)
	{
		// check initialise
		MYGUI_ASSERT(mIsInitialise, INSTANCE_TYPE_NAME << "used but not initialised");

		MYGUI_LOG(Info, "Uninstalling plugin: " << _plugin->getName());
		PluginList::iterator it = mPlugins.find(_plugin);
		if (it != mPlugins.end())
		{
			_plugin->shutdown();
			_plugin->uninstall();
			mPlugins.erase(it);
		}
		MYGUI_LOG(Info, "Plugin successfully uninstalled");
	}

	void PluginManager::unloadAllPlugins()
	{
		while (false == mLibs.empty())
			unloadPlugin((*mLibs.begin()).first);
	}

} // namespace MyGUI
