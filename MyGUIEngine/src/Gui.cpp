
#include "Gui.h"
#include "Widget.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(Gui);

	void Gui::initialise(Ogre::RenderWindow* _window)
	{
		assert(!mIsInitialise);

		// регистрируем фабрику текста и панели
		Ogre::OverlayManager &manager = Ogre::OverlayManager::getSingleton();
		mFactoryTextSimpleOverlay = new TextSimpleOverlayElementFactory();
		mFactoryTextEditOverlay = new TextEditOverlayElementFactory();
		mFactoryPanelAlphaOverlay = new PanelAlphaOverlayElementFactory();

		manager.addOverlayElementFactory(mFactoryTextSimpleOverlay);
		manager.addOverlayElementFactory(mFactoryTextEditOverlay);
		manager.addOverlayElementFactory(mFactoryPanelAlphaOverlay);

		Ogre::Viewport * port = _window->getViewport(0);
		mHeight = port->getActualHeight();
		mWidth = port->getActualWidth();

		// создаем и инициализируем синглтоны
		mInputManager = new InputManager();
		mInputManager->initialise();
		mInputManager->load("main.lang");

		mBasisWidgetManager = new BasisWidgetManager();
		mBasisWidgetManager->initialise();

		mClipboardManager = new ClipboardManager();

		mLayerManager = new LayerManager();
		mLayerManager->initialise();
		mLayerManager->load("main.layer");

		mSkinManager = new SkinManager();
		mSkinManager->initialise();
		mSkinManager->load("main.skin");

		mParserManager = new ParserManager();
		mParserManager->initialise();

		mWidgetManager = new WidgetManager();
		mWidgetManager->initialise();

		mLayoutManager = new LayoutManager();

		mFontManager = new FontManager();
		mFontManager->load("MyGUI.font");

		mPointerManager = new PointerManager();
		mPointerManager->initialise();
		mPointerManager->load("main.pointer");
		mPointerManager->show();

		mDynLibManager = new DynLibManager();
		mDynLibManager->initialise();

		mPluginManager = new PluginManager();
		mPluginManager->initialise();

		mIsInitialise = true;
	}

	void Gui::shutdown()
	{
		if (!mIsInitialise) return;

		destroyWidget();

		// деинициализируем и удаляем синглтоны
		mPluginManager->shutdown();
		delete mPluginManager;

		mDynLibManager->shutdown();
		delete mDynLibManager;

		mParserManager->shutdown();
		delete mParserManager;

		mWidgetManager->shutdown();
		delete mWidgetManager;


		mLayerManager->shutdown();
		delete mLayerManager;

		delete mPointerManager;

		delete mFontManager;

		delete mLayoutManager;

		mInputManager->shutdown();
		delete mInputManager;

		mSkinManager->shutdown();
		delete mSkinManager;


		mBasisWidgetManager->shutdown();
		delete mBasisWidgetManager;

		delete mClipboardManager;

		delete mFactoryTextSimpleOverlay;
		delete mFactoryTextEditOverlay;
		delete mFactoryPanelAlphaOverlay;

		mIsInitialise = false;
	}

	WidgetPtr Gui::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, Align _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _x, _y, _cx, _cy, _align, 0, _name);
		mWidgetChild.push_back(widget);
		// присоединяем виджет с уровню
		LayerManager::getInstance().attachItem(widget, _layer, true);
		return widget;
	}

	// удяляет только негодных батюшке государю
	void Gui::destroyWidget(WidgetPtr & _widget)
	{
		for (size_t index = 0; index < mWidgetChild.size(); index++) {
			WidgetPtr widget = mWidgetChild[index];
			if (_widget == widget) {
				// удаляем свое имя
				WidgetManager::getInstance().clearName(_widget);
				delete _widget;
				_widget = 0;

				// удаляем из списка
				mWidgetChild[index] = mWidgetChild[mWidgetChild.size()-1];
				mWidgetChild.pop_back();
				return;
			}
		}
	}

	// удаляет всех детей
	void Gui::destroyWidget()
	{
		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); iter++) {
			WidgetPtr widget = *iter;
			// удаляем свое имя
			WidgetManager::getInstance().clearName(widget);
			// отсоединяем виджет от уровня, если он был присоединен
			LayerManager::getInstance().detachItem(widget);
			// и удаляем
			delete widget;
		}
		mWidgetChild.clear();
	}

} // namespace MyGUI