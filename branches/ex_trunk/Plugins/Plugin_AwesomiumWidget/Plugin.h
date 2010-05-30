/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "AwesomiumWidget.h"
#include "MyGUI_Plugin.h"

/*!	Test plugin to demonstrate possibilities of plugins for MyGUI
*/
namespace plugin
{

	class Plugin : public MyGUI::IPlugin
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
		// фабрики виджетов
		Awesomium::AwesomiumWidgetFactory* mFactory;

		static const std::string LogSection;

	};

} // namespace plugin

#endif // __PLUGIN_H__