/*!
	@file		PluginManager.h
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/
#ifndef _PLUGINMANAGER_H_
#define _PLUGINMANAGER_H_


#include "Prerequest.h"
#include "Plugin.h"

#include <string>

typedef void (*DLL_START_PLUGIN)(void);
typedef void (*DLL_STOP_PLUGIN)(void);

namespace MyGUI
{

	

	/*!	\brief Plugin manager. Load/unload and register plugins.
	*/
	class _MyGUIExport PluginManager
	{
	private:
		PluginManager();

		~PluginManager();

	public:
		//!	Get pointer to instance
		static PluginManager* Instance();

		//!	Shut down
		static void shutdown();

		//!	Initialization
		void initialize();

	private:
		//!	Pointer to instance
		static PluginManager *m_instance;

	public:
		//!	Load plugin
		void loadPlugin(const std::string &fileName);

		//!	Unload plugin
		void unloadPlugin(const std::string &fileName);

		/*!	Install plugin
			
			@remarks Calls from plugin
		*/
		void installPlugin(Plugin *plugin);

		/*!	Uninstall plugin
			
			@remarks Calls from plugin
		*/
		void uninstallPlugin(Plugin *plugin);

		//!	Unload all plugins
		void unloadAllPlugins();

	private:
		//!	List of dynamic libraries
		typedef std::list <DynLib*> DynLibList;

		//!	List of plugins
		typedef std::list <Plugin*> PluginList;

		//!	Loaded libraries
		DynLibList mLibs;

		//!	Installed plugins
		PluginList mPlugins;

	};

}

#endif