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

	class Plugin : public MyGUI::IPlugin
	{
	public:
		//!	Initialization
		void initialize() override;

		//!	Installation
		void install() override;

		//!	Shut down
		void shutdown() override;

		//!	Uninstall
		void uninstall() override;

		//! Get name
		const std::string& getName() const override;

	private:
		static const std::string_view LogSection;
	};

} // namespace plugin

#endif // PLUGIN_H_
