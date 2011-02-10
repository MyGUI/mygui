/*!
	@file
	@author		George Evmenov
	@date		02/2011
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		m_button(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Common/Demos");
		addResourceLocation(getRootMedia() + "/Demos/Demo_PluginBerkeliumWidget");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Example of using berkelium plugin with basic browser implementation and usage of common functions.");

		const MyGUI::IntSize& view = MyGUI::RenderManager::getInstance().getViewSize();
		const MyGUI::IntSize size(300, 26);

	/*#ifdef _DEBUG
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_StrangeButton_d.dll");
	#else
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_StrangeButton.dll");
	#endif*/
	}

	void DemoKeeper::destroyScene()
	{
	#ifdef _DEBUG
		MyGUI::PluginManager::getInstance().unloadPlugin("Plugin_StrangeButton_d.dll");
	#else
		MyGUI::PluginManager::getInstance().unloadPlugin("Plugin_StrangeButton.dll");
	#endif
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
