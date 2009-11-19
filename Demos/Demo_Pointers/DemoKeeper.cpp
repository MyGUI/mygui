/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "ResourcePointerContext.h"

#ifdef MYGUI_SAMPLES_INPUT_WIN32
#    include "ResourceW32Pointer.h"
#endif

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mEnemyPanel(nullptr),
		mFriendPanel(nullptr),
		mControlPanel(nullptr),
		mPointerContextManager(nullptr),
		mRightButtonPressed(false)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_Pointers");
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		getGUI()->load("Wallpaper0.layout");
		MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("BackHelp.layout");
		root.at(0)->findWidget("Text")->setCaption("");

		MyGUI::FactoryManager::getInstance().registryFactory<ResourcePointerContext>("Resource");

		getGUI()->load("Contexts.xml");

#ifdef MYGUI_SAMPLES_INPUT_WIN32
		//setPointerManaged(false);
		MyGUI::FactoryManager::getInstance().registryFactory<ResourceW32Pointer>("Resource");
		getGUI()->load("Pointers_W32.xml");
#else
		getGUI()->load("Pointers.xml");
#endif

		mPointerContextManager = new PointerContextManager(this);
		mPointerContextManager->addContext("ptrx_Normal");
		mPointerContextManager->setPointer("normal");

		mEnemyPanel = new EnemyPanel();
		mFriendPanel = new FriendPanel();
		mControlPanel = new ControlPanel(mPointerContextManager);

		MyGUI::IntSize size = getGUI()->getViewSize();
		setMousePosition(size.width / 2, size.height / 2);
		updateCursorPosition();

	}

	void DemoKeeper::destroyScene()
	{
		delete mEnemyPanel;
		mEnemyPanel = nullptr;
		delete mFriendPanel;
		mFriendPanel = nullptr;
		delete mControlPanel;
		mControlPanel = nullptr;

		delete mPointerContextManager;
		mPointerContextManager = nullptr;

		MyGUI::FactoryManager::getInstance().unregistryFactory<ResourcePointerContext>("Resource");
#ifdef MYGUI_SAMPLES_INPUT_WIN32
		MyGUI::FactoryManager::getInstance().unregistryFactory<ResourceW32Pointer>("Resource");
#endif
	}

	void DemoKeeper::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (!getGUI())
			return;

		// при зажатой правой вращаем сцену
		if (mRightButtonPressed)
		{
			// относительное смещение
			int rel_x = _absx = mSaveCursorX;
			int rel_y = _absy = mSaveCursorY;

			_absx = mSaveCursorX;
			_absy = mSaveCursorY;
			setMousePosition(mSaveCursorX, mSaveCursorY);

			// вращаем сцену
		}
		else
		{
			// ввод мыши находить вне гу€
			if (!getGUI()->injectMouseMove(_absx, _absy, _absz))
			{
				// пикаем сцену, в нашем случае это картинки,
				// которые по сути не пикаютс€ как гуевые
				if (mFriendPanel->isIntersect(_absx, _absy))
				{
					mPointerContextManager->setPointer("friend");
				}
				else if (mEnemyPanel->isIntersect(_absx, _absy))
				{
					mPointerContextManager->setPointer("enemy");
				}
				else
				{
					// курсор не во что не попал в сцене
					mPointerContextManager->setPointer("normal");
				}
			}
		}
	}

	void DemoKeeper::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!getGUI())
			return;

		if (!getGUI()->injectMousePress(_absx, _absy, _id))
		{
			// вращаем сцену только когда не над гуем
			if (_id == MyGUI::MouseButton::Right)
			{
				mRightButtonPressed = true;
				mSaveCursorX = _absx;
				mSaveCursorY = _absy;
				setPointerVisible(false);
			}
		}
	}

	void DemoKeeper::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!getGUI())
			return;

		if (_id == MyGUI::MouseButton::Right)
		{
			mRightButtonPressed = false;
			setPointerVisible(true);
		}

		if (!getGUI()->injectMouseRelease(_absx, _absy, _id))
		{
		}
	}

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		// дл€ гор€чих клавиш
		mControlPanel->injectKeyPress(_key);

		base::BaseManager::injectKeyPress(_key, _text);
	}

	void DemoKeeper::setPointerName(const std::string& _name)
	{
#ifdef MYGUI_SAMPLES_INPUT_WIN32
		MyGUI::IResource* resource_generic = MyGUI::ResourceManager::getInstance().getByName(_name, false);
		if (resource_generic != nullptr)
		{
			ResourceW32Pointer* resource = resource_generic->castType<ResourceW32Pointer>(false);
			if (resource != nullptr)
			{
				//InputManager::setPointerName(resource->getPointer());
			}
		}
#else
		MyGUI::PointerManager::getInstance().setPointer(_name);
#endif
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)

