/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(Gui);

	void Gui::initialise(Ogre::RenderWindow* _window)
	{
		MYGUI_ASSERT(false == mIsInitialise);
		MYGUI_LOG("* Initialise: ", INSTANCE_TYPE_NAME);

		Ogre::Viewport * port = _window->getViewport(0);
		mHeight = port->getActualHeight();
		mWidth = port->getActualWidth();

		// ������������ ������� ������ � ������
		Ogre::OverlayManager &manager = Ogre::OverlayManager::getSingleton();
		mFactoryTextSimpleOverlay = new TextSimpleOverlayElementFactory();
		mFactoryTextEditOverlay = new TextEditOverlayElementFactory();
		mFactoryPanelAlphaOverlay = new PanelAlphaOverlayElementFactory();

		manager.addOverlayElementFactory(mFactoryTextSimpleOverlay);
		manager.addOverlayElementFactory(mFactoryTextEditOverlay);
		manager.addOverlayElementFactory(mFactoryPanelAlphaOverlay);

		// ������� � �������������� ���������
		mInputManager = new InputManager();
		mInputManager->initialise();
		mBasisWidgetManager = new BasisWidgetManager();
		mBasisWidgetManager->initialise();
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

		// ��������� ��������� ���������
		mInputManager->load("main.lang");
		mLayerManager->load("main.layer");
		mSkinManager->load("main.skin");
		mFontManager->load("MyGUI.font");
		mPluginManager->loadPluginCfg("MyGUI_plugins.cfg");
		mPointerManager->load("main.pointer");
		mPointerManager->show();

		Ogre::Root::getSingleton().addFrameListener(this);

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully initialized");
		mIsInitialise = true;
	}

	void Gui::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG("* Shutdown: ", INSTANCE_TYPE_NAME);

		Ogre::Root::getSingleton().removeFrameListener(this);
		mListFrameListener.clear();
		mListFrameListenerAdd.clear();

		destroyAllWidget();

		// ���������������� � ������� ���������
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
		mBasisWidgetManager->shutdown();
		delete mBasisWidgetManager;
		mClipboardManager->shutdown();
		delete mClipboardManager;

		delete mFactoryTextSimpleOverlay;
		delete mFactoryTextEditOverlay;
		delete mFactoryPanelAlphaOverlay;

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully shutdown");
		mIsInitialise = false;
	}

	WidgetPtr Gui::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _left, _top, _width, _height, _align, 0, _name);
		mWidgetChild.push_back(widget);
		// ������������ ������ � ������
		LayerManager::getInstance().attachItem(widget, _layer, true);
		return widget;
	}

	// ������� ������ �������� ������� ��������
	void Gui::destroyWidget(WidgetPtr & _widget)
	{
		for (size_t index = 0; index < mWidgetChild.size(); index++) {
			WidgetPtr widget = mWidgetChild[index];
			if (_widget == widget) {

				// ������� ���� ���
				WidgetManager::getInstance().clearName(_widget);

				// ������� ���������� � ������
				InputManager::getInstance().widgetUnlink(widget);

				delete _widget;
				_widget = 0;

				// ������� �� ������
				mWidgetChild[index] = mWidgetChild[mWidgetChild.size()-1];
				mWidgetChild.pop_back();
				return;
			}
		}
	}

	// ������� ���� �����
	void Gui::destroyAllWidget()
	{
		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); iter++) {
			WidgetPtr widget = *iter;
			// ������� ���� ���
			WidgetManager::getInstance().clearName(widget);
			// ����������� ������ �� ������, ���� �� ��� �����������
			LayerManager::getInstance().detachItem(widget);
			// ������� ���������� � ������
			InputManager::getInstance().widgetUnlink(widget);
			// � �������
			delete widget;
		}
		mWidgetChild.clear();
	}

	bool Gui::frameStarted(const Ogre::FrameEvent& evt)
	{
		// ������� ���������
		ListFrameListener::iterator iter=mListFrameListener.begin();
		while (iter != mListFrameListener.end()) {
			if (null == (*iter)) iter = mListFrameListener.erase(iter);
			else {
				(*iter)->frameStarted(evt);
				++iter;
			}
		};
		// ������ ������ ������
		if (false == mListFrameListenerAdd.empty()) {
			mListFrameListener.merge(mListFrameListenerAdd);
		}
		return true;
	}

	bool Gui::frameEnded(const Ogre::FrameEvent& evt)
	{
		return true;
	}

	void Gui::addFrameListener(Ogre::FrameListener * _listener)
	{
		if (null == _listener) return;

		for (ListFrameListener::iterator iter=mListFrameListener.begin(); iter!=mListFrameListener.end(); ++iter) {
			if ((*iter) == _listener) return;
		}
		for (ListFrameListener::iterator iter=mListFrameListenerAdd.begin(); iter!=mListFrameListenerAdd.end(); ++iter) {
			if ((*iter) == _listener) return;
		}
		mListFrameListenerAdd.push_back(_listener);
	}

	void Gui::removeFrameListener(Ogre::FrameListener * _listener)
	{
		if (null == _listener) return;

		bool find = false;
		for (ListFrameListener::iterator iter=mListFrameListener.begin(); iter!=mListFrameListener.end(); ++iter) {
			if ((*iter) == _listener) {
				(*iter) = null;
				find = true;
				break;
			}
		}

		if (false == find) {
			for (ListFrameListener::iterator iter=mListFrameListenerAdd.begin(); iter!=mListFrameListenerAdd.end(); ++iter) {
				if ((*iter) == _listener) {
					mListFrameListenerAdd.erase(iter);
					break;
				}
			}
		}

	}

} // namespace MyGUI