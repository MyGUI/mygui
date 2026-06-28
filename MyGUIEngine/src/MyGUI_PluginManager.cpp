/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_PluginManager.h"
#include "MyGUI_DynLibManager.h"
#include "MyGUI_ResourceManager.h"

#include <pugixml.hpp>

namespace MyGUI
{
	MYGUI_SINGLETON_DEFINITION(PluginManager);

	using DLL_START_PLUGIN = void (*)();
	using DLL_STOP_PLUGIN = void (*)();

	PluginManager::PluginManager() :
		mXmlPluginTagName("Plugin"),
		mSingletonHolder(this)
	{
	}

	void PluginManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		ResourceManager::getInstance().registerLoadXmlDelegate(mXmlPluginTagName) =
			newDelegate(this, &PluginManager::_load);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void PluginManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		unloadAllPlugins();
		ResourceManager::getInstance().unregisterLoadXmlDelegate(mXmlPluginTagName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	bool PluginManager::loadPlugin(std::string_view _file)
	{
#ifdef EMSCRIPTEN
		return false;
#endif
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " used but not initialised");

		// Load plugin library
		DynLib* lib = DynLibManager::getInstance().load(_file);
		if (!lib)
		{
			MYGUI_LOG(Error, "Plugin '" << _file << "' not found");
			return false;
		}

		// Call startup function
		DLL_START_PLUGIN pFunc = reinterpret_cast<DLL_START_PLUGIN>(lib->getSymbol("dllStartPlugin"));
		if (!pFunc)
		{
			MYGUI_LOG(Error, "Cannot find symbol 'dllStartPlugin' in library " << _file);
			return false;
		}

		// Store for later unload
		mLibs[lib->getName()] = lib;

		// This must call installPlugin
		pFunc();

		return true;
	}

	void PluginManager::unloadPlugin(std::string_view _file)
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " used but not initialised");

		DynLibList::iterator it = mLibs.find(_file);
		if (it != mLibs.end())
		{
			// Call plugin shutdown
			DLL_STOP_PLUGIN pFunc = reinterpret_cast<DLL_STOP_PLUGIN>((*it).second->getSymbol("dllStopPlugin"));

			MYGUI_ASSERT(
				nullptr != pFunc,
				getClassTypeName() << "Cannot find symbol 'dllStopPlugin' in library " << _file);

			// this must call uninstallPlugin
			pFunc();
			// Unload library (destroyed by DynLibManager)
			DynLibManager::getInstance().unload((*it).second);
			mLibs.erase(it);
		}
	}

	void PluginManager::_load(xml::ElementPtr _node, std::string_view /*_file*/, Version _version)
	{
		for (auto child : _node.node())
		{
			if (child.name() == std::string_view("path"))
			{
				std::string_view source = child.attribute("source").value();
				if (!source.empty())
					loadPlugin(source);
			}
			else if (child.name() == std::string_view("Plugin"))
			{
				std::string_view source;

				for (auto source_node : _node.node().children("Source"))
				{
					std::string_view build = source_node.attribute("build").value();
#if MYGUI_DEBUG_MODE == 1
					if (build == "Debug")
						source = source_node.text().as_string();
#else
					if (build != "Debug")
						source = source_node.text().as_string();
#endif
				}
				if (!source.empty())
					loadPlugin(source);
			}
		}
	}

	void PluginManager::installPlugin(IPlugin* _plugin)
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " used but not initialised");

		MYGUI_LOG(Info, "Installing plugin: " << _plugin->getName());

		mPlugins.insert(_plugin);
		_plugin->install();

		_plugin->initialize();

		MYGUI_LOG(Info, "Plugin successfully installed");
	}

	void PluginManager::uninstallPlugin(IPlugin* _plugin)
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " used but not initialised");

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
		while (!mLibs.empty())
			unloadPlugin((*mLibs.begin()).first);
	}

} // namespace MyGUI
