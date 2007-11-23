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

	// ������� �������
	mStrangeButtonFactory = new MyGUI::factory::StrangeButtonFactory();

	// ������� �������
	mStrangeButtonParser = new MyGUI::parser::StrangeButtonParser();

}

void TestPlugin::shutdown()
{

	// ������� �������
	delete mStrangeButtonParser;

	// ������� �������
	delete mStrangeButtonFactory;

}

const std::string& TestPlugin::getName() const
{
	static std::string type("TestPlugin");
	return type;
}