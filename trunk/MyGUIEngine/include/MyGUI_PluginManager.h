/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/
#ifndef __MYGUI_PLUGIN_MANAGER_H__
#define __MYGUI_PLUGIN_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include <Ogre.h>
#include "MyGUI_Instance.h"
#include "MyGUI_Plugin.h"
#include "MyGUI_XmlDocument.h"

typedef void (*DLL_START_PLUGIN)(void);
typedef void (*DLL_STOP_PLUGIN)(void);

namespace MyGUI
{

	/*!	\brief Plugin manager. Load/unload and register plugins.
	*/
	class _MyGUIExport PluginManager
	{
		INSTANCE_HEADER(PluginManager);

	public:
		//!	Initialization
		void initialise();
		//!	Shut down
		void shutdown();

	public:
		//!	Load plugin
		void loadPlugin(const std::string& _file);

		//!	Unload plugin
		void unloadPlugin(const std::string& _file);

		/** Load additional MyGUI *.pluggin file */
		bool load(const std::string& _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

		/*!	Install plugin
			
			@remarks Calls from plugin
		*/
		void installPlugin(Plugin* _plugin);

		/*!	Uninstall plugin
			
			@remarks Calls from plugin
		*/
		void uninstallPlugin(Plugin* _plugin);

		//!	Unload all plugins
		void unloadAllPlugins();

	private:
		//!	List of dynamic libraries
		typedef std::map <std::string, DynLib*> DynLibList;

		//!	List of plugins
		typedef std::set <Plugin*> PluginList;

		//!	Loaded libraries
		DynLibList mLibs;

		//!	Installed plugins
		PluginList mPlugins;

	};

}

#endif // __MYGUI_PLUGIN_MANAGER_H__
