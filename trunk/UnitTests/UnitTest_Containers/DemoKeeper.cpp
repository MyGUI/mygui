/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/

//#include "precompiled.h"
#include "DemoKeeper.h"
#include "MyGUI_FlowContainer.h"

namespace demo
{
	bool DemoKeeper::frameStarted(const Ogre::FrameEvent & evt)
	{
		bool res = BaseManager::frameStarted(evt);
		//MyGUI::PointerManager::getInstance().get

		//mFlow->update();

		return res;
	}

	
	bool DemoKeeper::mouseMoved(const OIS::MouseEvent &arg)
	{
		//MyGUI::MYGUI_OUT(arg.state.X.abs, ", ", arg.state.Y.abs);

		return BaseManager::mouseMoved(arg);
	}

	void DemoKeeper::clear()
	{
	}

	void DemoKeeper::xmlTest()
	{
		base::BaseManager::getInstance().addResourceLocation("../../Media/UnitTests/");
		//MyGUI::ResourceManager::getInstance().a
		bool result = mGUI->load("flow_container_test.xml");

		mFlow = mGUI->findWidget<MyGUI::FlowContainer>("Flow");

		mFlow->updateAllWidgetInfos();
		mFlow->update();

		int p = 0;
	}

	void DemoKeeper::codeTest()
	{
		MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(200, 200, 500, 500), MyGUI::Align::Default, "Overlapped");
		window->setCaption("FlowContainer");

		MyGUI::FlowContainerPtr flow = window->createWidget<MyGUI::FlowContainer>("FlowContainer", MyGUI::IntCoord(MyGUI::IntPoint(), window->getClientCoord().size()), MyGUI::Align::HStretch | MyGUI::Align::VStretch, "Overlapped");

		//flow->

		MyGUI::SpacerPtr lineBreak = nullptr;
		MyGUI::SpacerPtr spacer = nullptr;
		MyGUI::FlowContainer::WidgetInfo* info = nullptr;

		// strange tabs for grouping buttons and spacers

		//flow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, 70, 50), MyGUI::Align::Default)
		//	->setCaption("Button 1");

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

	void DemoKeeper::createScene()
    {
		// I love this theme :)
		mGUI->load("core_theme_black_orange.xml");
		mGUI->load("core_skin.xml");

		mFlowContainerFactory = new MyGUI::factory::FlowContainerFactory();
		mSpacerFactory = new MyGUI::factory::SpacerFactory();

		codeTest();
		//xmlTest();
	}	

	bool DemoKeeper::keyPressed(const OIS::KeyEvent &arg)
	{
		return base::BaseManager::keyPressed(arg);
	}

    void DemoKeeper::destroyScene()
    {
		delete mFlowContainerFactory;
		mFlowContainerFactory = 0;

		delete mSpacerFactory;
		mSpacerFactory = 0;
    }
	 
} // namespace demo
