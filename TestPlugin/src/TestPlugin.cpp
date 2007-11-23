#include "TestPlugin.h"
#include "StrangeButton.h"

TestPlugin::TestPlugin()
{
}

TestPlugin::~TestPlugin()
{
}

void TestPlugin::install()
{
}

void TestPlugin::uninstall()
{
}

void TestPlugin::initialize()
{

	// создаем фабрики
	mStrangeButtonFactory = new MyGUI::factory::StrangeButtonFactory();

	// создаем парсеры
	mStrangeButtonParser = new MyGUI::parser::StrangeButtonParser();

}

void TestPlugin::shutdown()
{

	// удаляем парсеры
	delete mStrangeButtonParser;

	// удаляем фабрику
	delete mStrangeButtonFactory;

}

const std::string& TestPlugin::getName() const
{
	static std::string type("TestPlugin");
	return type;
}