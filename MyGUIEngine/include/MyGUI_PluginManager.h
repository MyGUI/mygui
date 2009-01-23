/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/
#ifndef __MYGUI_PLUGIN_MANAGER_H__
#define __MYGUI_PLUGIN_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Plugin.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Version.h"
#include "MyGUI_ResourceManager.h"

#include <Ogre.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	typedef void (*DLL_START_PLUGIN)(void);
	typedef void (*DLL_STOP_PLUGIN)(void);

	/*!	\brief Plugin manager. Load/unload and register plugins.
	*/
	class MYGUI_EXPORT PluginManager
	{
		MYGUI_INSTANCE_HEADER(PluginManager);

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

		/** Load additional MyGUI *_plugin.xml file */
		bool load(const std::string& _file, const std::string & _group = MyGUI::ResourceManager::GUIResourceGroupName);
		void _load(xml::ElementPtr _node, const std::string & _file, Version _version);

		/*!	Install plugin
			
			@remarks Calls from plugin
		*/
		void installPlugin(IPlugin* _plugin);

		/*!	Uninstall plugin
			
			@remarks Calls from plugin
		*/
		void uninstallPlugin(IPlugin* _plugin);

		//!	Unload all plugins
		void unloadAllPlugins();

	private:
		//!	List of dynamic libraries
		typedef std::map <std::string, DynLib*> DynLibList;

		//!	List of plugins
		typedef std::set <IPlugin*> PluginList;

		//!	Loaded libraries
		DynLibList mLibs;

		//!	Installed plugins
		PluginList mPlugins;

	};

}

#endif // __MYGUI_PLUGIN_MANAGER_H__
