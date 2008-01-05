#include "TestPlugin.h"
#include "StrangeButton.h"
#include "MyGUI_WidgetManager.h"
//#include "MyGUI_LogManager.h"

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

//	MyGUI::LogManager::out("MyGUI::LogManager::General", "MyGUI::LogManager::Info") << "test plugin" << MyGUI::LogManager::info(__FILE__, __LINE__);// << MyGUI::LogManager::endl;
	//std::string test1 = MyGUI::LogManager::General;

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