#include "TestPlugin.h"
#include "StrangeButton.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LogManager.h"

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

	//const std::string Section = "TestPlugin";
	//MyGUI::LogManager::registerSection(Section, "TestPlugin.log");

	//MyGUI::LogManager::out(Section, "Info") << "test plugin" << MyGUI::LogManager::info(__FILE__, __LINE__) << MyGUI::endl;

	//std::string test1 = MyGUI::LogManager::General;

	// ВОТ ЭТО НИКАК =(
	//int test = MyGUI::LogManager::Test;

//	Ogre::String test = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

}

void TestPlugin::shutdown()
{

	// удаляем фабрику
	delete mStrangeButtonFactory;

}

const std::string& TestPlugin::getName() const
{
	static std::string type("TestPlugin");
	return type;
}