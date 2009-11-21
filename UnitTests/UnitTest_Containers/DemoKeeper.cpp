/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/

//#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "MyGUI_FlowContainer.h"

namespace demo
{
	/*bool DemoKeeper::frameStarted(const Ogre::FrameEvent & evt)
	{
		bool res = BaseManager::frameStarted(evt);
		//MyGUI::PointerManager::getInstance().get

		//mFlow->update();

		return res;
	}*/

	
	/*bool DemoKeeper::mouseMoved(const OIS::MouseEvent &arg)
	{
		//MyGUI::MYGUI_OUT(arg.state.X.abs, ", ", arg.state.Y.abs);

		return BaseManager::mouseMoved(arg);
	}*/

	void DemoKeeper::clear()
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/");
	}

	void DemoKeeper::xmlTest()
	{
		bool result = getGUI()->load("flow_container_test.xml");

		mFlow = getGUI()->findWidget<MyGUI::FlowContainer>("Flow");

		mFlow->updateAllWidgetInfos();
		mFlow->update();

		int p = 0;
	}

	void DemoKeeper::codeTest()
	{
		MyGUI::WindowPtr window = getGUI()->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(200, 200, 500, 500), MyGUI::Align::Default, "Overlapped");
		window->setCaption("FlowContainer");

		MyGUI::FlowContainerPtr flow = window->createWidget<MyGUI::FlowContainer>("FlowContainer", MyGUI::IntCoord(MyGUI::IntPoint(), window->getClientCoord().size()), MyGUI::Align::HStretch | MyGUI::Align::VStretch, "Overlapped");

		//flow->

		MyGUI::SpacerPtr lineBreak = nullptr;
		MyGUI::SpacerPtr spacer = nullptr;
		MyGUI::FlowContainer::WidgetInfo* info = nullptr;

		// strange tabs for grouping buttons and spacers

		/*MyGUI::ButtonPtr button = flow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, 70, 50), MyGUI::Align::Default)
		button->setCaption("Button 1");
		button->eventMouseButtonClick = MyGUI::delegates::newDelegate(this, &DemoKeeper::buttonClicked());*/

		//	spacer = flow->createWidget<MyGUI::Spacer>("Spacer", MyGUI::IntCoord(), MyGUI::Align::Default);
		//	info = flow->getWidgetInfo(spacer);
		//	info->size.w.fl(1.0, MyGUI::FM_FREE_SPACE);
		//	info->size.h.px(20);

		//flow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, 70, 50), MyGUI::Align::Default)
		//	->setCaption("Button 2");

		//	spacer = flow->createWidget<MyGUI::Spacer>("Spacer", MyGUI::IntCoord(), MyGUI::Align::Default);
		//	info = flow->getWidgetInfo(spacer);
		//	info->size.fl(0.3, 0, MyGUI::FM_FREE_SPACE);

		//flow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, 70, 50), MyGUI::Align::Default)
		//	->setCaption("Button 3");

		//flow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, 70, 50), MyGUI::Align::Default)
		//	->setCaption("Button 4");

		//	lineBreak = flow->createWidget<MyGUI::Spacer>("Spacer", MyGUI::IntCoord(), MyGUI::Align::Default);
		//	info = flow->getWidgetInfo(lineBreak);
		//	info->size.fl(0, 0.5, MyGUI::FM_FREE_SPACE);
		//	info->lineBreak = true;

		//	/*lineBreak = flow->createWidget<MyGUI::Spacer>("Spacer", MyGUI::IntCoord(0, 0, 0, 5), MyGUI::Align::Default);
		//	info = flow->getWidgetInfo(lineBreak);
		//	info->size.w.fl(1, MyGUI::FM_FREE_SPACE);
		//	info->lineBreak = false;*/

		//MyGUI::ButtonPtr b = flow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, 70, 50), MyGUI::Align::Default);
		//b->setCaption("Button 5");
		//info = flow->getWidgetInfo(b);
		//info->size.w.fl(0.5, MyGUI::FM_PARENT);
		//info->lineBreak = true;
		//
		//	lineBreak = flow->createWidget<MyGUI::Spacer>("Spacer", MyGUI::IntCoord(), MyGUI::Align::Default);
		//	info = flow->getWidgetInfo(lineBreak);
		//	info->size.fl(0, 0.5, MyGUI::FM_FREE_SPACE);
		//	info->lineBreak = false;

		//b = flow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, 70, 50), MyGUI::Align::Default);
		//b->setCaption("Button 6");

		//	lineBreak = flow->createWidget<MyGUI::Spacer>("Spacer", MyGUI::IntCoord(0, 0, 0, 10), MyGUI::Align::Default);
		//	info = flow->getWidgetInfo(lineBreak);
		//	info->size.w.fl(0.5, MyGUI::FM_FREE_SPACE);
		//	info->lineBreak = false;

		//b = flow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, 70, 50), MyGUI::Align::Default);
		//b->setCaption("Button 7");

		//	//spacer = flow->createWidget<MyGUI::Spacer>("Spacer", MyGUI::IntCoord(), MyGUI::Align::Default);
		//	//info = flow->getWidgetInfo(spacer);
		//	//info->size.fl(0.5, 0);

		//	lineBreak = flow->createWidget<MyGUI::Spacer>("Spacer", MyGUI::IntCoord(0, 0, 5, 5), MyGUI::Align::Default);
		//	info = flow->getWidgetInfo(lineBreak);
		//	info->size.px(MyGUI::INT_SIZE_UNBOUND.width, 5);
		//	info->lineBreak = true;

		for(int i = 0; i < 4; ++i)
		{
			MyGUI::ButtonPtr b = flow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, 70, 50), MyGUI::Align::Default);
			b->setCaption("Button " + Ogre::StringConverter::toString(i + 8));
			info = flow->getWidgetInfo(b);
			
				spacer = flow->createWidget<MyGUI::Spacer>("Spacer", MyGUI::IntCoord(0, 0, 50, 50), MyGUI::Align::Default);
				info = flow->getWidgetInfo(spacer);

			if(i % 4 == 3)
			{
				lineBreak = flow->createWidget<MyGUI::Spacer>("Spacer", MyGUI::IntCoord(0, 0, 0, 0), MyGUI::Align::Default);
				info = flow->getWidgetInfo(lineBreak);
				info->size.px(MyGUI::INT_SIZE_UNBOUND.width, 5);
				info->lineBreak = true;
				info->autoLineBreak = false;
			}
		}

		flow->updateAllWidgetInfos();

		flow->update();
	}

	void DemoKeeper::buttonClicked(MyGUI::Widget* _sender)
	{
		MyGUI::MYGUI_OUT("Button clicked!");
	}

	void DemoKeeper::createScene()
    {
		// I love this theme :)
		getGUI()->load("core_theme_black_orange.xml");
		getGUI()->load("core_skin.xml");

		MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
		factory.registryFactory<MyGUI::FlowContainer>("Widget");
		factory.registryFactory<MyGUI::Spacer>("Widget");
		codeTest();
		//xmlTest();
	}	

	/*bool DemoKeeper::keyPressed(const OIS::KeyEvent &arg)
	{
		return base::BaseManager::keyPressed(arg);
	}*/

    void DemoKeeper::destroyScene()
    {
    }
	 
} // namespace demo

MYGUI_APP(demo::DemoKeeper)
