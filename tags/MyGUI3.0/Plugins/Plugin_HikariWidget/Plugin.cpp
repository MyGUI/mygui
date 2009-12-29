/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/

#include "Plugin.h"
#include "MyGUI_LogManager.h"
#include "HikariWidget.h"
#include "KeyboardHookImpl.h"

namespace plugin
{

	const std::string Plugin::LogSection = "Plugin";
	HMODULE Plugin::msFlashLib = 0;

	Plugin::Plugin() : 
		mKeyboardHook(0)
	{
		MyGUI::LogManager::registerSection(Plugin::LogSection, MYGUI_LOG_FILENAME);
	}

	Plugin::~Plugin()
	{
		MyGUI::LogManager::unregisterSection(Plugin::LogSection);
	}

	void Plugin::install()
	{
	}

	void Plugin::uninstall()
	{
	}

	void Plugin::initialize()
	{
		MYGUI_LOGGING(LogSection, Info, "initialize");

		CoInitialize(0);
		const std::string flash_lib = "Flash.ocx";
		msFlashLib = LoadLibraryA(flash_lib.c_str());
		if (!msFlashLib)
		{
			MYGUI_LOGGING(LogSection, Error, flash_lib << " not found");
		}

		// создаем фабрики
		MyGUI::FactoryManager::getInstance().registryFactory<Hikari::HikariWidget>("Widget");

		mKeyboardHook = new KeyboardHook(&mHookListenerImpl);
	}

	void Plugin::shutdown()
	{
		MYGUI_LOGGING(LogSection, Info, "shutdown");

		delete mKeyboardHook;
		mKeyboardHook = 0;

		// удаляем фабрику
		MyGUI::FactoryManager::getInstance().unregistryFactory<Hikari::HikariWidget>("Widget");

		if (msFlashLib)
		{
			FreeLibrary(msFlashLib);
			msFlashLib = 0;
		}
		CoUninitialize();
	}

	const std::string& Plugin::getName() const
	{
		static std::string type("Plugin");
		return type;
	}

} // namespace plugin

plugin::Plugin* plugin_item = 0;

extern "C" MYGUI_EXPORT_DLL void dllStartPlugin(void)
{
	plugin_item = new plugin::Plugin();
	MyGUI::PluginManager::getInstance().installPlugin(plugin_item);
}

extern "C" MYGUI_EXPORT_DLL void dllStopPlugin(void)
{
	MyGUI::PluginManager::getInstance().uninstallPlugin(plugin_item);
	delete plugin_item;
	plugin_item = 0;
}