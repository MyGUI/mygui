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

	void Gui::initialise(Ogre::RenderWindow* _window,
		const std::string& _lang,
		const std::string& _layer,
		const std::string& _skin,
		const std::string& _font,
		const std::string& _pointer,
		const std::string& _plugin)
	{
		// самый первый лог
		LogManager::registerSection(MYGUI_LOG_SECTION, MYGUI_LOG_FILENAME);

		MYGUI_ASSERT(false == mIsInitialise, "initialise already");

		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);
		MYGUI_LOG(Info, "* MyGUI version "
			<< MYGUI_VERSION_MAJOR << "."
			<< MYGUI_VERSION_MINOR << "."
			<< MYGUI_VERSION_PATCH);

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
		mWidgetManager = new WidgetManager();
		mInputManager = new InputManager();
		mCroppedRectangleManager = new SubWidgetManager();
		mClipboardManager = new ClipboardManager();
		mLayerManager = new LayerManager();
		mSkinManager = new SkinManager();
		mLayoutManager = new LayoutManager();
		mFontManager = new FontManager();
		mPointerManager = new PointerManager();
		mDynLibManager = new DynLibManager();
		mPluginManager = new PluginManager();
		mControllerManager = new ControllerManager();

		mWidgetManager->initialise();
		mInputManager->initialise();
		mCroppedRectangleManager->initialise();
		mClipboardManager->initialise();
		mLayerManager->initialise();
		mSkinManager->initialise();
		mLayoutManager->initialise();
		mFontManager->initialise();
		mPointerManager->initialise();
		mDynLibManager->initialise();
		mPluginManager->initialise();
		mControllerManager->initialise();

		// загружаем дефолтные настройки
		mInputManager->load(_lang);
		mLayerManager->load(_layer);
		mSkinManager->load(_skin);
		mFontManager->load(_font);
		mPointerManager->load(_pointer);
		mPluginManager->load(_plugin);
		mPointerManager->show();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void Gui::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		mListFrameListener.clear();

		_destroyAllChildWidget();

		// деинициализируем и удаляем синглтоны
		mPluginManager->shutdown();
		mDynLibManager->shutdown();
		mWidgetManager->shutdown();
		mPointerManager->shutdown();
		mLayerManager->shutdown();
		mFontManager->shutdown();
		mLayoutManager->shutdown();
		mInputManager->shutdown();
		mSkinManager->shutdown();
		mCroppedRectangleManager->shutdown();
		mClipboardManager->shutdown();
		mControllerManager->shutdown();

		delete mPluginManager;
		delete mDynLibManager;
		delete mWidgetManager;
		delete mPointerManager;
		delete mLayerManager;
		delete mFontManager;
		delete mLayoutManager;
		delete mInputManager;
		delete mSkinManager;
		delete mCroppedRectangleManager;
		delete mClipboardManager;
		delete mControllerManager;

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
	}

	void Gui::addFrameListener(FrameListener * _listener)
	{
		if (null == _listener) return;

		removeFrameListener(_listener);
		mListFrameListener.push_back(_listener);
	}

	void Gui::removeFrameListener(FrameListener * _listener)
	{
		if (null == _listener) return;

		for (ListFrameListener::iterator iter=mListFrameListener.begin(); iter!=mListFrameListener.end(); ++iter) {
			if ((*iter) == _listener) {
				(*iter) = null;
				return;
			}
		}
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

} // namespace MyGUI
