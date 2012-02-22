/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/

#include "Plugin.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_FactoryManager.h"
#include "StrangeButton.h"

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

		// создаем фабрики
		MyGUI::FactoryManager::getInstance().registerFactory<StrangeButton>("Widget");
	}

	void Plugin::shutdown()
	{
		MYGUI_LOGGING(LogSection, Info, "shutdown");

		// удаляем фабрику
		MyGUI::FactoryManager::getInstance().unregisterFactory<StrangeButton>("Widget");
	}

	const std::string& Plugin::getName() const
	{
		static std::string type("Plugin");
		return type;
	}

} // namespace plugin
