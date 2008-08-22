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
	// загружаем ресурсы для демо
	// потом сделать и для мака
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../Media/Demos/Demo_PanelView", "FileSystem", "General");

	using namespace MyGUI;
	const IntSize & view = Gui::getInstance().getViewSize();
	const IntSize size(600, 300);

	m_view.initialise();

	m_view.addItem(&m_panel1);
	m_view.addItem(&m_panel2);
	m_view.addItem(&m_panel3);
	m_view.addItem(&m_panel4);

	m_panel1.setCaption("Panel 1");
	m_panel2.setCaption("Panel 2");
	m_panel3.setCaption("Panel 3");
	m_panel4.setCaption("Panel 4");

	//m_panel1.initialise(scroll_view);
	//m_panel2.initialise();

}

void DemoKeeper::end()
{
	m_view.shutdown();
	//m_panel.shutdown();
	//m_panel.shutdown();
}
