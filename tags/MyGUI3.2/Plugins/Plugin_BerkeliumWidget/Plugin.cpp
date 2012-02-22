/*!
	@file
	@author		Albert Semenov
	@date		02/2010
*/

#include "Plugin.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_PluginManager.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_Gui.h"
#include "BerkeliumWidget.h"
#include <berkelium/Berkelium.hpp>

namespace plugin
{

	const std::string Plugin::LogSection = "Plugin";

	Plugin::Plugin()
	{
	}

	Plugin::~Plugin()
	{
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

		Berkelium::init(Berkelium::FileString::empty());

		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::BerkeliumWidget>("Widget");
		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &Plugin::update);
	}

	void Plugin::shutdown()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &Plugin::update);
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::BerkeliumWidget>("Widget");

		Berkelium::destroy();

		MYGUI_LOGGING(LogSection, Info, "shutdown");
	}

	const std::string& Plugin::getName() const
	{
		static std::string type("Plugin");
		return type;
	}

	void Plugin::update(float _time)
	{
		Berkelium::update();
	}

} // namespace plugin

plugin::Plugin* plugin_item = nullptr;

extern "C" MYGUI_EXPORT_DLL void dllStartPlugin(void)
{
	plugin_item = new plugin::Plugin();
	MyGUI::PluginManager::getInstance().installPlugin(plugin_item);
}

extern "C" MYGUI_EXPORT_DLL void dllStopPlugin(void)
{
	MyGUI::PluginManager::getInstance().uninstallPlugin(plugin_item);
	delete plugin_item;
	plugin_item = nullptr;
}
