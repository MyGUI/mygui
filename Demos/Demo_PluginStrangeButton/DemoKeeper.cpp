/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		getGUI()->load("Wallpaper0.layout");
		MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("BackHelp.layout");
		root.at(0)->findWidget("Text")->setCaption("Example of using plugins in MyGUI. Actually nothing interesting to look at.");

		// если группа ресурсов не указанна, то ищется в корне приложения
	#ifdef _DEBUG
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_StrangeButton_d.dll");
	#else
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_StrangeButton.dll");
	#endif

		m_button = MyGUI::Gui::getInstance().createWidgetT("StrangeButton", "Button", MyGUI::IntCoord(), MyGUI::Align::Center, "Main");
		m_button->setCaption("Plugin StrangeButton demo");
		m_button->setMinSize(MyGUI::IntSize(300, 26));
		m_button->setSizePolicy(MyGUI::SizePolicy::Content);
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::Gui::getInstance().destroyChildWidget(m_button);

	#ifdef _DEBUG
		MyGUI::PluginManager::getInstance().unloadPlugin("Plugin_StrangeButton_d.dll");
	#else
		MyGUI::PluginManager::getInstance().unloadPlugin("Plugin_StrangeButton.dll");
	#endif
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
