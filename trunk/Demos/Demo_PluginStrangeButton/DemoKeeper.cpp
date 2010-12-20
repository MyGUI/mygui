/*!
	@file
	@author		Albert Semenov
	@date		08/2008
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
	}

	void DemoKeeper::createScene()
	{
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Example of using plugins in MyGUI. Actually nothing interesting to look at.");

		const MyGUI::IntSize& view = MyGUI::RenderManager::getInstance().getViewSize();
		const MyGUI::IntSize size(300, 26);

		// ���� ������ �������� �� ��������, �� ������ � ����� ����������
	#ifdef _DEBUG
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_StrangeButton_d.dll");
	#else
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_StrangeButton.dll");
	#endif

		MyGUI::Widget* widget = MyGUI::Gui::getInstance().createWidgetT("StrangeButton", "Button", MyGUI::IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), MyGUI::Align::Default, "Main");
		m_button = widget->castType<MyGUI::TextBox>();
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

MYGUI_APP(demo::DemoKeeper)
