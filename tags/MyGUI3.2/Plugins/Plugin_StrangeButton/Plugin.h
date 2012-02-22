/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "MyGUI_Plugin.h"

/*!	Test plugin to demonstrate possibilities of plugins for MyGUI
*/
namespace plugin
{

	class Plugin :
		public MyGUI::IPlugin
	{
	public:
		Plugin();
		~Plugin();

	public:
		//!	Initialization
		virtual void initialize();

		//!	Installation
		virtual void install();

		//!	Shut down
		virtual void shutdown();

		//!	Uninstall
		virtual void uninstall();

		//! Get name
		virtual const std::string& getName() const;

	private:
		static const std::string LogSection;
	};

} // namespace plugin

#endif // __PLUGIN_H__
