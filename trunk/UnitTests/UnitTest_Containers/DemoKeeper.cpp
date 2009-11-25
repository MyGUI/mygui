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


#include <sys/types.h>

//#ifdef MYGUI_OGRE_PLATFORM
//#error A
//#endif

namespace demo
{
	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(mRootMedia + "/UnitTests/UnitTest_Containers/xmltests");
	}

	bool DemoKeeper::_test(const std::string& _layoutFileName)
	{
		// Delete old flow if existed
		MyGUI::Widget* lastFlow = getGUI()->findWidget<MyGUI::FlowContainer>("Flow", false);

		// TODO: Deleting and showing results of tests are not compatible!
		if (lastFlow != nullptr)
			getGUI()->destroyWidget(lastFlow);

		// Load new, find and update
		MYGUI_ASSERT(getGUI()->load(_layoutFileName), "Can't load test lauout!");

		MyGUI::FlowContainer* flow = getGUI()->findWidget<MyGUI::FlowContainer>("Flow");

		MYGUI_ASSERT(flow, "No flow widget!");

		MyGUI::Widget* captionWidget = flow->getParent();

		MYGUI_ASSERT(captionWidget, "No first parent of flow! Where do you add flow?");

		if (captionWidget->getParent())
			captionWidget = captionWidget->getParent();

		//MYGUI_ASSERT(flow->getParent()->getParent(), "No flow widget parent of parent!");
			
		captionWidget->setCaption(_layoutFileName);		

		flow->updateAllWidgetInfos();
		flow->update();

		for (size_t iter = 0; iter < flow->getChildCount(); ++iter)
		{
			if (!_testWidget(flow->getChildAt(iter), _layoutFileName))
			{
				captionWidget->setCaption(captionWidget->getCaption() + " - #FF0000FAILED!");
				//MYGUI_EXCEPT(std::string() + "Test " + _layoutFileName + " failed!");
				return false;
			}
		}

		captionWidget->setCaption(captionWidget->getCaption() + " - #00FF00passed!");

		return true;
	}

	bool DemoKeeper::_testWidget(MyGUI::Widget* _widget, const std::string& _testName)
	{
		// I hope
		MyGUI::FlowContainer* flow = MyGUI::castWidget<MyGUI::FlowContainer>(_widget->getParent());

		MyGUI::FlowContainer::WidgetInfo* info = flow->getWidgetInfo(_widget);

		MyGUI::IntCoord reqWidgetCoords = getFlowWidgetCoords(_widget);

		if(reqWidgetCoords == MyGUI::IntCoord())
		{
			MYGUI_EXCEPT("Required coord property wasn't found (or nil) in test " + 
				_testName + "/" + _widget->getName() + "!");
		}

		MyGUI::IntCoord readWidgetCoords = _widget->getCoord();

		return reqWidgetCoords == readWidgetCoords;
	}

	MyGUI::IntCoord DemoKeeper::getFlowWidgetCoords(MyGUI::Widget* _widget)
	{
		MyGUI::IntCoord reqWidgetCoords;

		// "MGIT_" - prefix for MyGUI Internal's Tests

		if (_widget->isUserString("MGIT_Coord"))
		{
			reqWidgetCoords = MyGUI::IntCoord::parse(_widget->getUserString("MGIT_Coord"));
			return reqWidgetCoords;
		}

		if (_widget->isUserString("MGIT_Pos") && _widget->isUserString("MGIT_Size"))
		{
			MyGUI::IntPoint point;
			point = MyGUI::IntPoint::parse(_widget->getUserString("MGIT_Pos"));

			MyGUI::IntSize size;
			size = MyGUI::IntSize::parse(_widget->getUserString("MGIT_Size"));

			return MyGUI::IntCoord(point, size);
		}

		return reqWidgetCoords;
	}

	void DemoKeeper::runXmlTests()
	{
		MyGUI::VectorString files = MyGUI::DataManager::getInstance().getDataListNames("*.xml");

		for (MyGUI::VectorString::iterator iter = files.begin(); iter != files.end(); ++iter)
		{
			if ( (*iter).find("unit_test_") != std::string::npos 
				&& (*iter).find(".xml") != std::string::npos )
			{
				_test(*iter);
			}
		}		
	}

// TODO: DELETE THIS MASS! SORRY FOR CAPS LOCK! :))
// - No, please wait me! Five_stars
#if 0 
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
			b->setCaption("Button " + MyGUI::utility::toString(i + 8));
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
#endif

	void DemoKeeper::createScene()
    {
		// I love this theme :)
		MyGUI::LanguageManager::getInstance().loadUserTags("core_theme_black_orange_tag.xml");
		getGUI()->load("core_skin.xml");

		MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
		factory.registryFactory<MyGUI::FlowContainer>("Widget");
		factory.registryFactory<MyGUI::Spacer>("Widget");

		runXmlTests();
	}	
	 
} // namespace demo

MYGUI_APP(demo::DemoKeeper)
