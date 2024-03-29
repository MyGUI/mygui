/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_PLUGIN_MANAGER_H_
#define MYGUI_PLUGIN_MANAGER_H_

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
	class MYGUI_EXPORT PluginManager : public MemberObsolete<PluginManager>
	{
		MYGUI_SINGLETON_DECLARATION(PluginManager);

	public:
		PluginManager();

		void initialise();
		void shutdown();

		//!	Load plugin
		bool loadPlugin(std::string_view _file);

		//!	Unload plugin
		void unloadPlugin(std::string_view _file);

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
		void _load(xml::ElementPtr _node, std::string_view _file, Version _version);

	private:
		//!	List of dynamic libraries
		using DynLibList = std::map<std::string, DynLib*, std::less<>>;

		//!	List of plugins
		using PluginList = std::set<IPlugin*>;

		//!	Loaded libraries
		DynLibList mLibs;

		//!	Installed plugins
		PluginList mPlugins;

		bool mIsInitialise{false};
		std::string mXmlPluginTagName;
	};

} // namespace MyGUI

#endif // MYGUI_PLUGIN_MANAGER_H_
