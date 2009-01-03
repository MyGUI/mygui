/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
 
namespace demo
{

	DemoKeeper::DemoKeeper() :
		mInformationWindow(null),
		mColourWindow(null),
		mNode(null)
	{
	}
 
    void DemoKeeper::createScene()
    {
		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Gui");
        //base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
        //base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");

		Ogre::Entity* entity = this->mSceneMgr->createEntity("axes.mesh", "axes.mesh");
		mNode = this->mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mNode->attachObject(entity);

		this->mCamera->setPosition(20, 20, 20);

		mGUI->eventFrameStart += MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);

		mMainPanel.initialise();
		mMainPanel.eventAction = MyGUI::newDelegate(this, &DemoKeeper::notifyEventAction);
		mMainPanel.addObject("FrameWindow");
		mMainPanel.addObject("Horizontal Scrollbar");
		mMainPanel.addObject("Vertical Scrollbar");
		mMainPanel.addObject("StaticText");
		mMainPanel.addObject("StaticImage");
		mMainPanel.addObject("Render to Texture");

		mEditorWindow.initialise();
    }
 
    void DemoKeeper::destroyScene()
    {
		mGUI->eventFrameStart -= MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);

		destroyWindows();
		mEditorWindow.shutdown();
		mMainPanel.shutdown();
    }

	void DemoKeeper::notifyFrameStart(float _time)
	{
		if (mNode) mNode->yaw(Ogre::Radian(Ogre::Degree(_time * 10)));
	}

	void DemoKeeper::destroyWindows()
	{
		if (mInformationWindow) {
			mInformationWindow->shutdown();
			delete mInformationWindow;
			mInformationWindow = null;
		}
		if (mColourWindow) {
			mColourWindow->shutdown();
			delete mColourWindow;
			mColourWindow = null;
		}
	}

	void DemoKeeper::createWindows(wraps::BaseLayout& _parent)
	{
		destroyWindows();

		mInformationWindow = new InformationWindow();
		mInformationWindow->initialise(_parent.mainWidget());

		mColourWindow = new ColourWindow();
		mColourWindow->initialise(_parent.mainWidget());
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
			mEditorWindow.clearView();
		}
		else if (_action == MainPanel::EventLoad) {
			createWindows(mEditorWindow);
		}
		else if (_action == MainPanel::EventCreate) {

			const MyGUI::IntCoord& coord = mEditorWindow->getClientCoord();

			if (_index == 0) {
				const MyGUI::IntSize size(80, 80);
				MyGUI::WindowPtr window = mEditorWindow->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowCS", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				window->setCaption("Frame");
				window->setMinSize(size.width, size.height);
			}
			else if (_index == 1) {
				const MyGUI::IntSize size(180, 15);
				MyGUI::HScrollPtr scroll = mEditorWindow->createWidget<MyGUI::HScroll>("HScroll", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				scroll->setScrollRange(200);
				scroll->setScrollPosition(10);
				scroll->setScrollPage(1);
				scroll->setScrollViewPage(20);
			}
			else if (_index == 2) {
				const MyGUI::IntSize size(15, 180);
				MyGUI::VScrollPtr scroll = mEditorWindow->createWidget<MyGUI::VScroll>("VScroll", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				scroll->setScrollRange(200);
				scroll->setScrollPosition(10);
				scroll->setScrollPage(1);
				scroll->setScrollViewPage(20);
			}
			else if (_index == 3) {
				const MyGUI::IntSize size(80, 26);
				MyGUI::StaticTextPtr text = mEditorWindow->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				text->setCaption("StaticText");
			}
			else if (_index == 4) {
				const MyGUI::IntSize size(50, 50);
				MyGUI::StaticImagePtr image = mEditorWindow->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				image->setImageInfo("core.png", MyGUI::IntCoord(50, 203, 50, 50), MyGUI::IntSize(50, 50));
				image->setImageIndex(0);
			}
			else if (_index == 5) {
				const MyGUI::IntSize size(150, 150);
				MyGUI::WindowPtr window = mEditorWindow->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowC", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				window->setCaption("Render");
				MyGUI::RenderBoxPtr box = window->createWidget<MyGUI::RenderBox>("RenderBox", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);
				box->setRenderTarget(this->mCamera);
				box->getViewport()->setOverlaysEnabled(false);
			}
		}
	}

} // namespace demo
