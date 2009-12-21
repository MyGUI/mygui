/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

#ifdef MYGUI_OGRE_PLATFORM

#include "Ogre/RenderBox/RenderBox.h"

namespace demo
{
	std::vector<wraps::RenderBox*> mRenderBoxes;
}

#endif // MYGUI_OGRE_PLATFORM

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mInformationWindow(nullptr),
		mColourWindow(nullptr),
		mMainPanel(nullptr),
		mEditorWindow(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_Gui");
		addResourceLocation(getRootMedia() + "/Common/Scene");
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		createDefaultScene();
		MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("BackHelp.layout");
		root.at(0)->findWidget("Text")->setCaption("Demonstration of using different widgets and styles (something like Ogre Demo_Gui).");

		mMainPanel = new MainPanel();
		mMainPanel->eventAction = MyGUI::newDelegate(this, &DemoKeeper::notifyEventAction);
		mMainPanel->addObject("FrameWindow");
		mMainPanel->addObject("Horizontal Scrollbar");
		mMainPanel->addObject("Vertical Scrollbar");
		mMainPanel->addObject("StaticText");
		mMainPanel->addObject("StaticImage");
		mMainPanel->addObject("Render to Texture");

		mEditorWindow = new EditorWindow();
    }

	void DemoKeeper::destroyScene()
	{
		destroyWindows();

		delete mEditorWindow;
		mEditorWindow = nullptr;
		delete mMainPanel;
		mMainPanel = nullptr;
	}

	void DemoKeeper::destroyWindows()
	{
		delete mInformationWindow;
		mInformationWindow = nullptr;

		delete mColourWindow;
		mColourWindow = nullptr;
	}

	void DemoKeeper::createWindows()
	{
		destroyWindows();

		mInformationWindow = new InformationWindow(mEditorWindow->getView());
		mColourWindow = new ColourWindow(mEditorWindow->getView());
	}

	int getRand(int _min, int _max)
	{
		if (_max < _min) std::swap(_max, _min);
		int range = _max - _min;
		if (range == 0) return 0;
		int result = ::rand() % range;
		if (result < 0) result = -result;
		return _min + result;
	}

	void DemoKeeper::notifyEventAction(MainPanel::TypeEvents _action, size_t _index)
	{
		if (_action == MainPanel::EventQuit)
		{
			quit();
		}
		else if (_action == MainPanel::EventNew)
		{

#ifdef MYGUI_OGRE_PLATFORM

			for (std::vector<wraps::RenderBox*>::iterator item=mRenderBoxes.begin(); item!=mRenderBoxes.end(); ++item)
			{
				delete *item;
			}
			mRenderBoxes.clear();

#endif // MYGUI_OGRE_PLATFORM

			destroyWindows();
			mEditorWindow->clearView();
		}
		else if (_action == MainPanel::EventLoad)
		{
			createWindows();
		}
		else if (_action == MainPanel::EventCreate)
		{
			MyGUI::Widget* view = mEditorWindow->getView();
			const MyGUI::IntCoord& coord = view->getClientCoord();

			if (_index == 0)
			{
				const MyGUI::IntSize size(80, 80);
				MyGUI::Window* window = view->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowCS", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				window->setCaption("Frame");
				window->setMinSize(size.width, size.height);
			}
			else if (_index == 1)
			{
				const MyGUI::IntSize size(180, 15);
				MyGUI::HScroll* scroll = view->createWidget<MyGUI::HScroll>("HScroll", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				scroll->setScrollRange(200);
				scroll->setScrollPosition(10);
				scroll->setScrollPage(1);
				scroll->setScrollViewPage(20);
			}
			else if (_index == 2)
			{
				const MyGUI::IntSize size(15, 180);
				MyGUI::VScroll* scroll = view->createWidget<MyGUI::VScroll>("VScroll", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				scroll->setScrollRange(200);
				scroll->setScrollPosition(10);
				scroll->setScrollPage(1);
				scroll->setScrollViewPage(20);
			}
			else if (_index == 3)
			{
				const MyGUI::IntSize size(80, 26);
				MyGUI::StaticText* text = view->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				text->setCaption("StaticText");
			}
			else if (_index == 4)
			{
				const MyGUI::IntSize size(50, 50);
				MyGUI::StaticImage* image = view->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				image->setImageInfo("core.png", MyGUI::IntCoord(50, 203, 50, 50), MyGUI::IntSize(50, 50));
				image->setImageIndex(0);
			}
			else if (_index == 5)
			{
				const MyGUI::IntSize size(150, 150);
				MyGUI::Window* window = view->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowC", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				window->setCaption("Render");
				MyGUI::Canvas* canvas = window->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);

#ifdef MYGUI_OGRE_PLATFORM

				wraps::RenderBox* box = new wraps::RenderBox();
				box->setCanvas(canvas);
				box->setViewport(getCamera());
				box->setBackgroundColour(MyGUI::Colour::Black);
				mRenderBoxes.push_back(box);

#endif // MYGUI_OGRE_PLATFORM

			}
		}
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
