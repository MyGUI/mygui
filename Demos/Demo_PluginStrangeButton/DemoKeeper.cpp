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

	ButtonPtr button = castWidget<Button>(Gui::getInstance().createWidgetT("StrangeButton", "Button", IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), ALIGN_DEFAULT, "Main"));
	button->setCaption("Plugin StrangeButton demo");

}

void DemoKeeper::end()
{
	using namespace MyGUI;

	Gui::getInstance().destroyAllChildWidget();

#ifdef _DEBUG
	PluginManager::getInstance().unloadPlugin("Plugin_StrangeButton_d.dll");
#else
	PluginManager::getInstance().unloadPlugin("Plugin_StrangeButton.dll");
#endif
}
