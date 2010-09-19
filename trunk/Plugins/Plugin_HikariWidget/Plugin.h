/*!
	@file
	@author		Albert Semenov
	@date		10/2009
*/

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "HikariWidget.h"
#include "MyGUI_Plugin.h"
#include "KeyboardHookImpl.h"

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

		static HMODULE getLibHandle()
		{
			return msFlashLib;
		}

	public:
		static const std::string LogSection;

	private:
		static HMODULE msFlashLib;
		HookListenerImpl mHookListenerImpl;
		KeyboardHook* mKeyboardHook;

	};

} // namespace plugin

#endif // __PLUGIN_H__
