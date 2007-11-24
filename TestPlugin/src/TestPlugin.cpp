#include "TestPlugin.h"
#include "StrangeButton.h"
#include "WidgetManager.h"

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

	// ������� �������
	mStrangeButtonFactory = new MyGUI::factory::StrangeButtonFactory();

}

void TestPlugin::shutdown()
{

	// ������� �������
	delete mStrangeButtonFactory;

}

const std::string& TestPlugin::getName() const
{
	static std::string type("TestPlugin");
	return type;
}