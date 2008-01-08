/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_Widget.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(Gui);

	void Gui::initialise(Ogre::RenderWindow* _window)
	{
		// самый первый лог
		LogManager::registerSection(MYGUI_LOG_SECTION, MYGUI_LOG_FILENAME);

		MYGUI_ASSERT(false == mIsInitialise, "initialise already");

		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		Ogre::Viewport * port = _window->getViewport(0);
		mViewSize.set(port->getActualWidth(), port->getActualHeight());

		// регистрируем фабрику текста и панели
		Ogre::OverlayManager &manager = Ogre::OverlayManager::getSingleton();
		mFactoryTextSimpleOverlay = new TextSimpleOverlayElementFactory();
		mFactoryTextEditOverlay = new TextEditOverlayElementFactory();
		mFactoryPanelAlphaOverlay = new PanelAlphaOverlayElementFactory();
		mFactorySharedPanelAlphaOverlay = new SharedPanelAlphaOverlayElementFactory();

		manager.addOverlayElementFactory(mFactoryTextSimpleOverlay);
		manager.addOverlayElementFactory(mFactoryTextEditOverlay);
		manager.addOverlayElementFactory(mFactoryPanelAlphaOverlay);
		manager.addOverlayElementFactory(mFactorySharedPanelAlphaOverlay);

		// создаем и инициализируем синглтоны
		mInputManager = new InputManager();
		mInputManager->initialise();
		mCroppedRectangleManager = new SubWidgetManager();
		mCroppedRectangleManager->initialise();
		mClipboardManager = new ClipboardManager();
		mClipboardManager->initialise();
		mLayerManager = new LayerManager();
		mLayerManager->initialise();
		mSkinManager = new SkinManager();
		mSkinManager->initialise();
		mWidgetManager = new WidgetManager();
		mWidgetManager->initialise();
		mLayoutManager = new LayoutManager();
		mLayoutManager->initialise();
		mFontManager = new FontManager();
		mFontManager->initialise();
		mPointerManager = new PointerManager();
		mPointerManager->initialise();
		mDynLibManager = new DynLibManager();
		mDynLibManager->initialise();
		mPluginManager = new PluginManager();
		mPluginManager->initialise();

		// загружаем дефолтные настройки
		mInputManager->load("main.lang");
		mLayerManager->load("main.layer");
		mSkinManager->load("main.skin");
		mFontManager->load("MyGUI.font");
		mPluginManager->loadPluginCfg("MyGUI_plugins.cfg");
		mPointerManager->load("main.pointer");
		mPointerManager->show();

		// создаем контроллеры
		mControllerFadeAlpha = new ControllerFadeAlpha();
		mVectorControllers.push_back(mControllerFadeAlpha);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void Gui::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		mListFrameListener.clear();
		mListFrameListenerAdd.clear();

		_destroyAllChildWidget();

		// удаляем контроллеры
		mVectorControllers.clear();
		delete mControllerFadeAlpha;

		// деинициализируем и удаляем синглтоны
		mPluginManager->shutdown();
		delete mPluginManager;
		mDynLibManager->shutdown();
		delete mDynLibManager;
		mWidgetManager->shutdown();
		delete mWidgetManager;
		mPointerManager->shutdown();
		delete mPointerManager;
		mLayerManager->shutdown();
		delete mLayerManager;
		mFontManager->shutdown();
		delete mFontManager;
		mLayoutManager->shutdown();
		delete mLayoutManager;
		mInputManager->shutdown();
		delete mInputManager;
		mSkinManager->shutdown();
		delete mSkinManager;
		mCroppedRectangleManager->shutdown();
		delete mCroppedRectangleManager;
		mClipboardManager->shutdown();
		delete mClipboardManager;

		delete mFactoryTextSimpleOverlay;
		delete mFactoryTextEditOverlay;
		delete mFactoryPanelAlphaOverlay;
		delete mFactorySharedPanelAlphaOverlay;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");

		// самый последний лог
		LogManager::shutdown();

		mIsInitialise = false;
	}

	WidgetPtr Gui::createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _coord, _align, 0, _name);
		mWidgetChild.push_back(widget);
		// присоединяем виджет с уровню
		LayerManager::getInstance().attachItem(widget, _layer, true);
		return widget;
	}

	WidgetPtr Gui::findWidgetT(const std::string& _name)
    {
        return mWidgetManager->findWidgetT(_name);
    }

	void Gui::injectFrameEntered(Ogre::Real timeSinceLastFrame)
	{
		// сначала рассылаем
		ListFrameListener::iterator iter=mListFrameListener.begin();
		while (iter != mListFrameListener.end()) {
			if (null == (*iter)) iter = mListFrameListener.erase(iter);
			else {
				(*iter)->_frameEntered(timeSinceLastFrame);
				++iter;
			}
		};

		// теперь объединяем масив
		if (false == mListFrameListenerAdd.empty()) {
			for (ListFrameListener::iterator iter=mListFrameListenerAdd.begin(); iter!=mListFrameListenerAdd.end(); ++iter) {
				mListFrameListener.push_back(*iter);
			}
			mListFrameListenerAdd.clear();
		}
	}

	bool Gui::addFrameListener(FrameListener * _listener)
	{
		if (null == _listener) return false;

		for (ListFrameListener::iterator iter=mListFrameListener.begin(); iter!=mListFrameListener.end(); ++iter) {
			if ((*iter) == _listener) return false;
		}
		for (ListFrameListener::iterator iter=mListFrameListenerAdd.begin(); iter!=mListFrameListenerAdd.end(); ++iter) {
			if ((*iter) == _listener) return false;
		}
		mListFrameListenerAdd.push_back(_listener);
		return true;
	}

	bool Gui::removeFrameListener(FrameListener * _listener)
	{
		if (null == _listener) return false;

		for (ListFrameListener::iterator iter=mListFrameListener.begin(); iter!=mListFrameListener.end(); ++iter) {
			if ((*iter) == _listener) {
				(*iter) = null;
				return true;
			}
		}

		for (ListFrameListener::iterator iter=mListFrameListenerAdd.begin(); iter!=mListFrameListenerAdd.end(); ++iter) {
			if ((*iter) == _listener) {
				mListFrameListenerAdd.erase(iter);
				return true;
			}
		}
		return false;
	}


	// удяляет только негодных батюшке государю
	void Gui::_destroyChildWidget(WidgetPtr _widget)
	{
		for (size_t index = 0; index < mWidgetChild.size(); index++) {
			WidgetPtr widget = mWidgetChild[index];
			if (_widget == widget) {
				delete _widget;
				// удаляем из списка
				mWidgetChild[index] = mWidgetChild[mWidgetChild.size()-1];
				mWidgetChild.pop_back();
				return;
			}
		}
		MYGUI_EXCEPT("Widget is not find");
	}

	// удаляет всех детей
	void Gui::_destroyAllChildWidget()
	{
		while (false == mWidgetChild.empty()) {
			// отсылаем первый, так как он быстрее найдется в массиве
			// а удаление в векторе производится перестановкой, т.е. быстро
			WidgetPtr widget = mWidgetChild.front();
			WidgetManager::getInstance().destroyWidget(widget);
		}
	}

	void Gui::destroyAllWidget()
	{
		mWidgetManager->destroyAllWidget();
	}

	void Gui::destroyWidget(WidgetPtr _widget)
	{
		mWidgetManager->destroyWidget(_widget);
	}

	void Gui::_unlinkWidget(WidgetPtr _widget)
	{
		for (VectorControllerInterface::iterator iter=mVectorControllers.begin(); iter!=mVectorControllers.end(); ++iter) {
			(*iter)->_unlinkWidget(_widget);
		}
	}

} // namespace MyGUI
