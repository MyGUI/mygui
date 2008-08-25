/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"
#include "BasisManager.h"

void DemoKeeper::start()
{
	using namespace MyGUI;
	const IntSize & view = Gui::getInstance().getViewSize();
	const IntSize size(300, 26);

	// если группа ресурсов не указанна, то ищется в корне приложения
#ifdef _DEBUG
	PluginManager::getInstance().loadPlugin("Plugin_StrangeButton_d.dll");
#else
	PluginManager::getInstance().loadPlugin("Plugin_StrangeButton.dll");
#endif

	m_button = Gui::getInstance().createWidgetT("StrangeButton", "Button", IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), Align::Default, "Main");
	m_button->setCaption("Plugin StrangeButton demo");

}

void DemoKeeper::end()
{
	using namespace MyGUI;

	Gui::getInstance().destroyChildWidget(m_button);

#ifdef _DEBUG
	PluginManager::getInstance().unloadPlugin("Plugin_StrangeButton_d.dll");
#else
	PluginManager::getInstance().unloadPlugin("Plugin_StrangeButton.dll");
#endif
}
