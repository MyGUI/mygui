/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/

#ifndef PLUGIN_H_
#define PLUGIN_H_

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

#endif // PLUGIN_H_
