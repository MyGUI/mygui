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
		mTextBoxTest(nullptr)
	{
	}

	void DemoKeeper::notifyMouseSetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
	{
		MyGUI::ImageBox* image = _sender->castType<MyGUI::ImageBox>();
		image->setItemName("Offline");
		image->setVisible(true);
		mTextBoxTest->setCaption(_sender->getName());
	}

	void DemoKeeper::notifyMouseLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new)
	{
		MyGUI::ImageBox* image = _sender->castType<MyGUI::ImageBox>();
		image->setItemName("Dead");
		image->setVisible(true);
		mTextBoxTest->setCaption("0");
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_ChildAttach");
		addResourceLocation(getRootMedia() + "/Common/Demos");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
		MyGUI::ResourceManager::getInstance().load("SquadSlotPortraitIcons.xml");

		createSuccesScene();
		//	раскоментировать для того чтобы увидеть странное поведение (предыдущую строчку закоментить)
		createFailedScene();
	}

	void DemoKeeper::destroyScene()
	{
	}

	void DemoKeeper::createSuccesScene()
	{
		const MyGUI::VectorWidgetPtr& squad_roots = MyGUI::LayoutManager::getInstance().loadLayout("Squad.layout");
		const MyGUI::VectorWidgetPtr& squad_slot_roots = MyGUI::LayoutManager::getInstance().loadLayout("SquadSlot.layout");

		squad_roots.at(0)->setPosition(100, 100);

		mTextBoxTest = squad_roots.at(0)->findWidget("TextBoxTest")->castType<MyGUI::TextBox>();

		squad_slot_roots.at(0)->attachToWidget(squad_roots.at(0), MyGUI::WidgetStyle::Overlapped);
		squad_slot_roots.at(1)->attachToWidget(squad_roots.at(0), MyGUI::WidgetStyle::Overlapped);

		MyGUI::ImageBox* image = squad_slot_roots.at(0)->findWidget("SquadSlotPortrait")->castType<MyGUI::ImageBox>();

		image->eventMouseSetFocus += MyGUI::newDelegate(this, &DemoKeeper::notifyMouseSetFocus);
		image->eventMouseLostFocus += MyGUI::newDelegate(this, &DemoKeeper::notifyMouseLostFocus);
	}

	void DemoKeeper::createFailedScene()
	{
		const MyGUI::VectorWidgetPtr& squad_roots = MyGUI::LayoutManager::getInstance().loadLayout("Squad.layout");
		const MyGUI::VectorWidgetPtr& squad_slot_roots = MyGUI::LayoutManager::getInstance().loadLayout("SquadSlotFailed.layout");

		squad_roots.at(0)->setPosition(500, 100);

		//mTextBoxTest = squad_roots.at(0)->findWidget("TextBoxTest")->castType<MyGUI::TextBox>();

		squad_slot_roots.at(0)->attachToWidget(squad_roots.at(0));

		MyGUI::ImageBox* image = squad_slot_roots.at(0)->findWidget("SquadSlotPortrait")->castType<MyGUI::ImageBox>();

		image->eventMouseSetFocus += MyGUI::newDelegate(this, &DemoKeeper::notifyMouseSetFocus);
		image->eventMouseLostFocus += MyGUI::newDelegate(this, &DemoKeeper::notifyMouseLostFocus);
	}


} // namespace demo

MYGUI_APP(demo::DemoKeeper)
