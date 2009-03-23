/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{

		base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		base::BaseManager::getInstance().setDescriptionText("Example of using plugins in MyGUI. Actually nothing interesting to look at.");

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const MyGUI::IntSize size(300, 26);

		// если группа ресурсов не указанна, то ищется в корне приложения
	#ifdef _DEBUG
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_StrangeButton_d.dll");
	#else
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_StrangeButton.dll");
	#endif

		m_button = MyGUI::Gui::getInstance().createWidgetT("StrangeButton", "Button", MyGUI::IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), MyGUI::Align::Default, "Main");
		m_button->setCaption("Plugin StrangeButton demo");

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
