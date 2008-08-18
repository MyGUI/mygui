#include "Plugin_StrangeButton.h"
#include "StrangeButton.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LogManager.h"

const std::string TestPlugin::LogSection = "TestPlugin";

TestPlugin::TestPlugin()
{
	MyGUI::LogManager::registerSection(TestPlugin::LogSection, MYGUI_LOG_FILENAME);
}

TestPlugin::~TestPlugin()
{
	MyGUI::LogManager::unregisterSection(TestPlugin::LogSection);
}

void TestPlugin::install()
{
}

void TestPlugin::uninstall()
{
}

void TestPlugin::initialize()
{

	LOGGING(LogSection, Info, "initialize");

	// создаем фабрики
	mStrangeButtonFactory = new MyGUI::factory::StrangeButtonFactory();

}

void TestPlugin::shutdown()
{
	LOGGING(LogSection, Info, "shutdown");

	// удаляем фабрику
	delete mStrangeButtonFactory;

}

const std::string& TestPlugin::getName() const
{
	static std::string type("TestPlugin");
	return type;
}