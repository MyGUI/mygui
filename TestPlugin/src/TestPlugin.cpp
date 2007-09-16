#include "TestPlugin.h"
#include "StrangeButton.h"

TestPlugin::TestPlugin()
{

}

TestPlugin::~TestPlugin()
{

}

void TestPlugin::initialize()
{
	
}

void TestPlugin::install()
{
	// ������� ������� ��� ����� �������
	MyGUI::WidgetFactory<MyGUI::StrangeButton> *ButtonFactoryInstance = new MyGUI::WidgetFactory<MyGUI::StrangeButton>("StrangeButton");
}

void TestPlugin::shutDown()
{

}

void TestPlugin::uninstall()
{
	
}

MyGUI::String TestPlugin::getName() const
{
	return "TestPlugin";
}