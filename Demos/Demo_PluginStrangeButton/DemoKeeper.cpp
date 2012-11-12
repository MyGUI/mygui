/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

#ifdef MYGUI_STATIC
#include "Plugin.h"
plugin::Plugin* plugin_item = nullptr;
#endif

#if MYGUI_DEBUG_MODE
	#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		#define plugin_name "Plugin_StrangeButton_d.dll"
	#else
		#define plugin_name "libPlugin_StrangeButton_d.so"
	#endif
#else
	#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		#define plugin_name "Plugin_StrangeButton.dll"
	#else
		#define plugin_name "libPlugin_StrangeButton.so"
	#endif
#endif

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
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Example of using plugins in MyGUI. Actually nothing interesting to look at.");

		const MyGUI::IntSize& view = MyGUI::RenderManager::getInstance().getViewSize();
		const MyGUI::IntSize size(300, 26);

#ifdef MYGUI_STATIC
		plugin_item = new plugin::Plugin();
		MyGUI::PluginManager::getInstance().installPlugin(plugin_item);
#else
		MyGUI::PluginManager::getInstance().loadPlugin(plugin_name);
#endif

		MyGUI::Widget* widget = MyGUI::Gui::getInstance().createWidgetT("StrangeButton", "Button", MyGUI::IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), MyGUI::Align::Default, "Main");
		m_button = widget->castType<MyGUI::TextBox>();
		m_button->setCaption("Plugin StrangeButton demo");
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::Gui::getInstance().destroyChildWidget(m_button);

#ifdef MYGUI_STATIC
		MyGUI::PluginManager::getInstance().uninstallPlugin(plugin_item);
		delete plugin_item;
		plugin_item = nullptr;
#else
#	ifdef _DEBUG
		MyGUI::PluginManager::getInstance().unloadPlugin("Plugin_StrangeButton_d.dll");
#	else
		MyGUI::PluginManager::getInstance().unloadPlugin("Plugin_StrangeButton.dll");
#	endif
#endif
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
