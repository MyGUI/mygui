/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
 
namespace demo
{
 
    void DemoKeeper::createScene()
    {
        base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Gui");
        base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");

		mMainPanel.initialise();
		mMainPanel.eventAction = MyGUI::newDelegate(this, &DemoKeeper::notifyEventAction);
		mMainPanel.addObject("FrameWindow");
		mMainPanel.addObject("Horizontal Scrollbar");
		mMainPanel.addObject("Vertical Scrollbar");
		mMainPanel.addObject("StaticText");
		mMainPanel.addObject("StaticImage");
		mMainPanel.addObject("Render to Texture");

		mEditorWindow.initialise();
		mInformationWindow.initialise();
		mItemBoxWindow.initialise();
    }
 
    void DemoKeeper::destroyScene()
    {
		mEditorWindow.shutdown();
		mMainPanel.shutdown();
		mInformationWindow.shutdown();
		mItemBoxWindow.shutdown();
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
		}
		else if (_action == MainPanel::EventLoad) {
		}
		else if (_action == MainPanel::EventCreate) {

			MyGUI::WidgetPtr client = mEditorWindow->getClientWidget();

			if (_index == 0) {
				const MyGUI::IntSize size(80, 80);
				MyGUI::WindowPtr window = client->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(getRand(0, client->getWidth() - size.width), getRand(0, client->getHeight() - size.height), size.width, size.height), MyGUI::Align::Default);
				window->setMinSize(size.width, size.height);
			}
			else if (_index == 1) {
				const MyGUI::IntSize size(180, 15);
				MyGUI::HScrollPtr scroll = client->createWidget<MyGUI::HScroll>("HScroll", MyGUI::IntCoord(getRand(0, client->getWidth() - size.width), getRand(0, client->getHeight() - size.height), size.width, size.height), MyGUI::Align::Default);
				scroll->setScrollRange(200);
				scroll->setScrollPosition(10);
				scroll->setScrollPage(1);
				scroll->setScrollViewPage(20);
			}
			else if (_index == 2) {
				const MyGUI::IntSize size(15, 180);
				MyGUI::VScrollPtr scroll = client->createWidget<MyGUI::VScroll>("VScroll", MyGUI::IntCoord(getRand(0, client->getWidth() - size.width), getRand(0, client->getHeight() - size.height), size.width, size.height), MyGUI::Align::Default);
				scroll->setScrollRange(200);
				scroll->setScrollPosition(10);
				scroll->setScrollPage(1);
				scroll->setScrollViewPage(20);
			}
			else if (_index == 3) {
				const MyGUI::IntSize size(80, 26);
				MyGUI::StaticTextPtr text = client->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(getRand(0, client->getWidth() - size.width), getRand(0, client->getHeight() - size.height), size.width, size.height), MyGUI::Align::Default);
				text->setCaption("StaticText");
			}
			else if (_index == 4) {
				const MyGUI::IntSize size(50, 50);
				MyGUI::StaticImagePtr image = client->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(getRand(0, client->getWidth() - size.width), getRand(0, client->getHeight() - size.height), size.width, size.height), MyGUI::Align::Default);
				image->setImageInfo("core.png", MyGUI::IntCoord(50, 203, 50, 50), MyGUI::IntSize(50, 50));
				image->setImageIndex(0);
			}
		}
	}

} // namespace demo
