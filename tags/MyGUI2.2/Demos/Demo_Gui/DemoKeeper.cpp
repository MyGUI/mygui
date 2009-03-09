/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "DemoKeeper.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mInformationWindow(nullptr),
		mColourWindow(nullptr),
		mNode(nullptr)
	{
	}

	void DemoKeeper::createScene()
	{
		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Gui");
		//base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		//base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		base::BaseManager::getInstance().setDescriptionText("Demonstration of using different widgets and styles (something like Ogre Demo_Gui).");

		Ogre::Entity* entity = this->mSceneMgr->createEntity("axes.mesh", "axes.mesh");
		mNode = this->mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mNode->attachObject(entity);

		this->mCamera->setPosition(20, 20, 20);

		mGUI->eventFrameStart += MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);

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
		mGUI->eventFrameStart -= MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);

		destroyWindows();
		delete mEditorWindow;
		delete mMainPanel;
    }

	void DemoKeeper::notifyFrameStart(float _time)
	{
		if (mNode) mNode->yaw(Ogre::Radian(Ogre::Degree(_time * 10)));
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
		if (_action == MainPanel::EventQuit) {
			m_exit = true;
		}
		else if (_action == MainPanel::EventNew) {
			destroyWindows();
			mEditorWindow->clearView();
		}
		else if (_action == MainPanel::EventLoad) {
			createWindows();
		}
		else if (_action == MainPanel::EventCreate) {

			MyGUI::WidgetPtr view = mEditorWindow->getView();
			const MyGUI::IntCoord& coord = view->getClientCoord();

			if (_index == 0) {
				const MyGUI::IntSize size(80, 80);
				MyGUI::WindowPtr window = view->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowCS", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				window->setCaption("Frame");
				window->setMinSize(size.width, size.height);
			}
			else if (_index == 1) {
				const MyGUI::IntSize size(180, 15);
				MyGUI::HScrollPtr scroll = view->createWidget<MyGUI::HScroll>("HScroll", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				scroll->setScrollRange(200);
				scroll->setScrollPosition(10);
				scroll->setScrollPage(1);
				scroll->setScrollViewPage(20);
			}
			else if (_index == 2) {
				const MyGUI::IntSize size(15, 180);
				MyGUI::VScrollPtr scroll = view->createWidget<MyGUI::VScroll>("VScroll", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				scroll->setScrollRange(200);
				scroll->setScrollPosition(10);
				scroll->setScrollPage(1);
				scroll->setScrollViewPage(20);
			}
			else if (_index == 3) {
				const MyGUI::IntSize size(80, 26);
				MyGUI::StaticTextPtr text = view->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				text->setCaption("StaticText");
			}
			else if (_index == 4) {
				const MyGUI::IntSize size(50, 50);
				MyGUI::StaticImagePtr image = view->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				image->setImageInfo("core.png", MyGUI::IntCoord(50, 203, 50, 50), MyGUI::IntSize(50, 50));
				image->setImageIndex(0);
			}
			else if (_index == 5) {
				const MyGUI::IntSize size(150, 150);
				MyGUI::WindowPtr window = view->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowC", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				window->setCaption("Render");
				MyGUI::RenderBoxPtr box = window->createWidget<MyGUI::RenderBox>("RenderBox", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);
				box->setCamera(this->mCamera);
				box->setBackgroungColour(Ogre::ColourValue::Black);
				//box->getViewport()->setOverlaysEnabled(false);
			}
		}
	}

} // namespace demo
