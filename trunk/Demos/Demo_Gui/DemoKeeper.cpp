/*!
	@file
	@author     Albert Semenov
	@date       08/2008
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

#ifdef MYGUI_OGRE_PLATFORM

#include "Ogre/RenderBox/RenderBox.h"

namespace demo
{
	static std::vector<wraps::RenderBox*> mRenderBoxes;
}

#endif // MYGUI_OGRE_PLATFORM

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mEditorWindow(nullptr),
		mMainPanel(nullptr),
		mInformationWindow(nullptr),
		mColourWindow(nullptr)
	{
#ifdef MYGUI_OGRE_PLATFORM
		mNode = nullptr;
#endif // MYGUI_OGRE_PLATFORM
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_Gui");
		addResourceLocation(getRootMedia() + "/Common/Scene");
		addResourceLocation(getRootMedia() + "/Common/Demos");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		createDefaultScene();
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		if (root.size() == 1)
			root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Demonstration of using different widgets and styles (something like Ogre Demo_Gui).");

		mMainPanel = new MainPanel();
		mMainPanel->eventAction = MyGUI::newDelegate(this, &DemoKeeper::notifyEventAction);
		mMainPanel->addObject("FrameWindow");
		mMainPanel->addObject("Horizontal Scrollbar");
		mMainPanel->addObject("Vertical Scrollbar");
		mMainPanel->addObject("TextBox");
		mMainPanel->addObject("ImageBox");
		mMainPanel->addObject("Render to Texture");

		mEditorWindow = new EditorWindow();

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);

		removeRenderBoxes();
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

	static int getRand(int _min, int _max)
	{
		if (_max < _min)
			std::swap(_max, _min);
		int range = _max - _min;
		if (range == 0)
			return 0;
		int result = ::rand() % range;
		if (result < 0)
			result = -result;
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
			removeRenderBoxes();
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
				window->setMinSize(size);
			}
			else if (_index == 1)
			{
				const MyGUI::IntSize size(180, 15);
				MyGUI::ScrollBar* scroll = view->createWidget<MyGUI::ScrollBar>("ScrollBarH", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				scroll->setScrollRange(200);
				scroll->setScrollPosition(10);
				scroll->setScrollPage(1);
				scroll->setScrollViewPage(20);
			}
			else if (_index == 2)
			{
				const MyGUI::IntSize size(15, 180);
				MyGUI::ScrollBar* scroll = view->createWidget<MyGUI::ScrollBar>("ScrollBarV", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				scroll->setScrollRange(200);
				scroll->setScrollPosition(10);
				scroll->setScrollPage(1);
				scroll->setScrollViewPage(20);
			}
			else if (_index == 3)
			{
				const MyGUI::IntSize size(80, 26);
				MyGUI::TextBox* text = view->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				text->setCaption("TextBox");
			}
			else if (_index == 4)
			{
				const MyGUI::IntSize size(50, 50);
				MyGUI::ImageBox* image = view->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				image->setImageTexture("HelpIcon.png");
			}
			else if (_index == 5)
			{
				const MyGUI::IntSize size(150, 150);
				MyGUI::Window* window = view->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowC", MyGUI::IntCoord(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height), size.width, size.height), MyGUI::Align::Default);
				window->setCaption("Render");
				MyGUI::Canvas* canvas = window->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);

				createRenderBox(canvas);
			}
		}
	}

	void DemoKeeper::removeRenderBoxes()
	{
#ifdef MYGUI_OGRE_PLATFORM
		for (std::vector<wraps::RenderBox*>::iterator item = mRenderBoxes.begin(); item != mRenderBoxes.end(); ++item)
			delete *item;
		mRenderBoxes.clear();
#endif // MYGUI_OGRE_PLATFORM
	}

	void DemoKeeper::createRenderBox(MyGUI::Canvas* _canvas)
	{
#ifdef MYGUI_OGRE_PLATFORM
		wraps::RenderBox* box = new wraps::RenderBox();
		box->setCanvas(_canvas);
		box->setViewport(getCamera());
		box->setBackgroundColour(MyGUI::Colour::Black);
		mRenderBoxes.push_back(box);
#endif // MYGUI_OGRE_PLATFORM
	}

	void DemoKeeper::createDefaultScene()
	{
#ifdef MYGUI_OGRE_PLATFORM
		try
		{
			Ogre::Entity* entity = getSceneManager()->createEntity("Mikki.mesh", "Mikki.mesh");
			mNode = getSceneManager()->getRootSceneNode()->createChildSceneNode();
			mNode->attachObject(entity);
		}
		catch (Ogre::FileNotFoundException&)
		{
			return;
		}

		try
		{
			Ogre::MeshManager::getSingleton().createPlane(
				"FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 1000, 1000, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

			Ogre::Entity* entity = getSceneManager()->createEntity("FloorPlane", "FloorPlane");
			entity->setMaterialName("Ground");
			mNode->attachObject(entity);
		}
		catch (Ogre::FileNotFoundException&)
		{
		}
#endif // MYGUI_OGRE_PLATFORM
	}

	void DemoKeeper::notifyFrameStart(float _time)
	{
#ifdef MYGUI_OGRE_PLATFORM
		if (mNode)
			mNode->yaw(Ogre::Radian(Ogre::Degree(_time * 10)));
#endif // MYGUI_OGRE_PLATFORM
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
