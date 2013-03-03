/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_PLUGIN_H__
#define __MYGUI_PLUGIN_H__

#include "MyGUI_Prerequest.h"
#include <string>

namespace MyGUI
{

	/*!	\brief Base plugin class
	*/
	class MYGUI_EXPORT IPlugin
	{
	public:
		IPlugin() { }
		virtual ~IPlugin() { }

		/*!	Get the name of the plugin.
			@remarks An implementation must be supplied for this method to uniquely
			identify the plugin
		*/
		virtual const std::string& getName() const = 0;

		/*!	Perform the plugin initial installation sequence
		*/
		virtual void install() = 0;

		/*! Perform any tasks the plugin needs to perform on full system
			initialisation.
		*/
		virtual void initialize() = 0;

		/*!	Perform any tasks the plugin needs to perform when the system is shut down
		*/
		virtual void shutdown() = 0;

		/*!	Perform the final plugin uninstallation sequence
		*/
		virtual void uninstall() = 0;
	};

} // namespace MyGUI

#endif // __MYGUI_PLUGIN_H__
