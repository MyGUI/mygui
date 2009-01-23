/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/

#include "Plugin.h"
#include "MyGUI_LogManager.h"

namespace plugin
{

	const std::string Plugin::LogSection = "Plugin";

	Plugin::Plugin()
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

		// создаем фабрики
		mStrangeButtonFactory = new factory::StrangeButtonFactory();

	}

	void Plugin::shutdown()
	{
		MYGUI_LOGGING(LogSection, Info, "shutdown");

		// удаляем фабрику
		delete mStrangeButtonFactory;

	}

	const std::string& Plugin::getName() const
	{
		static std::string type("Plugin");
		return type;
	}

} // namespace plugin
