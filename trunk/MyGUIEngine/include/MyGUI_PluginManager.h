/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/
/*
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
#ifndef __MYGUI_PLUGIN_MANAGER_H__
#define __MYGUI_PLUGIN_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_Plugin.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Version.h"
#include "MyGUI_DynLib.h"
#include <set>
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	/*!	\brief Plugin manager. Load/unload and register plugins.
	*/
	class MYGUI_EXPORT PluginManager :
		public Singleton<PluginManager>,
		public MemberObsolete<PluginManager>
	{
	public:
		PluginManager();

		void initialise();
		void shutdown();

		//!	Load plugin
		bool loadPlugin(const std::string& _file);

		//!	Unload plugin
		void unloadPlugin(const std::string& _file);

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
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

	private:
		//!	List of dynamic libraries
		typedef std::map <std::string, DynLib*> DynLibList;

		//!	List of plugins
		typedef std::set <IPlugin*> PluginList;

		//!	Loaded libraries
		DynLibList mLibs;

		//!	Installed plugins
		PluginList mPlugins;

		bool mIsInitialise;
		std::string mXmlPluginTagName;
	};

} // namespace MyGUI

#endif // __MYGUI_PLUGIN_MANAGER_H__
