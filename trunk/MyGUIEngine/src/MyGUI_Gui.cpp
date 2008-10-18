/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_Widget.h"

#include "MyGUI_InputManager.h"
#include "MyGUI_SubWidgetManager.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_PluginManager.h"
#include "MyGUI_DynLibManager.h"
#include "MyGUI_DelegateManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_ResourceManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(Gui);

	void Gui::initialise(Ogre::RenderWindow* _window, const std::string& _core, const Ogre::String & _group)
	{
		// ����� ������ ���
		LogManager::registerSection(MYGUI_LOG_SECTION, MYGUI_LOG_FILENAME);

		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");

		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);
		MYGUI_LOG(Info, "* MyGUI version "
			<< MYGUI_VERSION_MAJOR << "."
			<< MYGUI_VERSION_MINOR << "."
			<< MYGUI_VERSION_PATCH);

		// ��������� �������
		mActiveViewport = 0;
		// ��������� ���� � �������
		mWindow = _window;
		mViewSize.set(mWindow->getViewport(mActiveViewport)->getActualWidth(), mWindow->getViewport(mActiveViewport)->getActualHeight());

		MYGUI_LOG(Info, "Viewport : " << mViewSize.print());

		// ������� � �������������� ���������
		mResourceManager = new ResourceManager();
		mLayerManager = new LayerManager();
		mWidgetManager = new WidgetManager();
		mInputManager = new InputManager();
		mSubWidgetManager = new SubWidgetManager();
		mSkinManager = new SkinManager();
		mFontManager = new FontManager();
		mControllerManager = new ControllerManager();
		mPointerManager = new PointerManager();
		mClipboardManager = new ClipboardManager();
		mLayoutManager = new LayoutManager();
		mDynLibManager = new DynLibManager();
		mPluginManager = new PluginManager();
		mDelegateManager = new DelegateManager();
		mLanguageManager = new LanguageManager();

		mResourceManager->initialise(_group);
		mLayerManager->initialise();
		mWidgetManager->initialise();
		mInputManager->initialise();
		mSubWidgetManager->initialise();
		mSkinManager->initialise();
		mFontManager->initialise();
		mControllerManager->initialise();
		mPointerManager->initialise();
		mClipboardManager->initialise();
		mLayoutManager->initialise();
		mDynLibManager->initialise();
		mPluginManager->initialise();
		mDelegateManager->initialise();
		mLanguageManager->initialise();

		WidgetManager::getInstance().registerUnlinker(this);

		// ������������� �� ��������� �������� ���� � ����� ���������
		Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
		windowResized(mWindow);

		// ��������� ��������� ��������� ���� ����
		if ( _core.empty() == false ) mResourceManager->load(_core, mResourceManager->getResourceGroup());

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void Gui::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		// ����� ������������
		Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
		WidgetManager::getInstance().unregisterUnlinker(this);

		_destroyAllChildWidget();

		// ���������������� � ������� ���������
		mPointerManager->shutdown();
		mWidgetManager->shutdown();
		mInputManager->shutdown();
		mSkinManager->shutdown();
		mSubWidgetManager->shutdown();
		mLayerManager->shutdown();
		mFontManager->shutdown();
		mControllerManager->shutdown();
		mClipboardManager->shutdown();
		mLayoutManager->shutdown();
		mPluginManager->shutdown();
		mDynLibManager->shutdown();
		mDelegateManager->shutdown();
		mLanguageManager->shutdown();
		mResourceManager->shutdown();

		delete mPointerManager;
		delete mWidgetManager;
		delete mInputManager;
		delete mSkinManager;
		delete mSubWidgetManager;
		delete mLayerManager;
		delete mFontManager;
		delete mControllerManager;
		delete mClipboardManager;
		delete mLayoutManager;
		delete mDynLibManager;
		delete mPluginManager;
		delete mDelegateManager;
		delete mLanguageManager;
		delete mResourceManager;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");

		// ����� ��������� ���
		LogManager::shutdown();

		mIsInitialise = false;
	}

	bool Gui::injectMouseMove( int _absx, int _absy, int _absz) {return mInputManager->injectMouseMove(_absx, _absy, _absz);}
	bool Gui::injectMousePress( int _absx, int _absy, MouseButton _id ) {return mInputManager->injectMousePress(_absx, _absy, _id);}
	bool Gui::injectMouseRelease( int _absx, int _absy, MouseButton _id ) {return mInputManager->injectMouseRelease(_absx, _absy, _id);}

	bool Gui::injectKeyPress(KeyCode _key) {return mInputManager->injectKeyPress(_key);}
	bool Gui::injectKeyRelease(KeyCode _key) {return mInputManager->injectKeyRelease(_key);}

#ifndef MYGUI_NO_OIS
	bool Gui::injectKeyPress(const OIS::KeyEvent & _arg) { return mInputManager->injectKeyPress((KeyCode)_arg.key, _arg.text); }
#endif

	WidgetPtr Gui::_createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _coord, _align, null, this, _name);
		mWidgetChild.push_back(widget);
		// ������������ ������ � ������
		if (false == _layer.empty()) LayerManager::getInstance().attachToLayerKeeper(_layer, widget);
		return widget;
	}

	WidgetPtr Gui::findWidgetT(const std::string& _name)
	{
		return mWidgetManager->findWidgetT(_name);
	}

	// ������� ����������
	void Gui::_destroyChildWidget(WidgetPtr _widget)
	{
		MYGUI_ASSERT(null != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end()) {

			// ��������� ���������
			MyGUI::WidgetPtr widget = *iter;

			// ������� �� ������
			*iter = mWidgetChild.back();
			mWidgetChild.pop_back();

			// ���������� �� ����
			mWidgetManager->unlinkFromUnlinkers(_widget);

			// ���������������� ��������
			_deleteWidget(widget);
		}
		else MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
	}

	// ������� ���� �����
	void Gui::_destroyAllChildWidget()
	{
		while (false == mWidgetChild.empty()) {

			// ����� ���� ���������, ����� ��������� �������� ������� ���
			WidgetPtr widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			// ���������� �� ����
			mWidgetManager->unlinkFromUnlinkers(widget);

			// � ���� ������, ��� ��� ��� ������ � ������ ���
			_deleteWidget(widget);
		}
	}

	bool Gui::load(const std::string & _file, const std::string & _group)
	{
		return mResourceManager->load(_file, _group);
	}

	// ��� ���������� �� ��������� ���� �������
	void Gui::windowResized(Ogre::RenderWindow* rw)
	{
		FloatSize oldViewSize = mViewSize;

		Ogre::Viewport * port = rw->getViewport(mActiveViewport);
		mViewSize.set(port->getActualWidth(), port->getActualHeight());
		mLayerManager->_windowResized(mViewSize);

		// ����������� ������� ����
		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			_alignWidget((*iter), oldViewSize, mViewSize);
		}
	}

	void Gui::destroyWidget(WidgetPtr _widget)
	{
		mWidgetManager->destroyWidget(_widget);
	}

	void Gui::destroyWidgetsVector(VectorWidgetPtr _widgets)
	{
		mWidgetManager->destroyWidgetsVector(_widgets);
	}

	void Gui::_alignWidget(WidgetPtr _widget, const FloatSize& _old, const FloatSize& _new)
	{
		if (null == _widget) return;
		
		Align align = _widget->getAlign();
		if (align.isDefault()) return;

		IntCoord coord = _widget->getCoord();

		// �������������� ������������
		if (align.isHStretch()) {
			// �����������
			coord.width += _new.width - _old.width;
		}
		else if (align.isRight()) {
			// ������� �� ������� ����
			coord.left += _new.width - _old.width;
		}
		else if (align.isHCenter()) {
			// ������������ �� ����������� ��� ����������
			coord.left = (_new.width - coord.width) / 2;
		}

		if (align.isVStretch()) {
			// �����������
			coord.height += _new.height - _old.height;
		}
		else if (align.isBottom()) {
			// ������� �� ������� ����
			coord.top += _new.height - _old.height;
		}
		else if (align.isVCenter()) {
			// ������������ �� ��������� ��� ����������
			coord.top = (_new.height - coord.height) / 2;
		}

		_widget->setCoord(coord);
	}

	void Gui::hidePointer()
	{
		mPointerManager->hide();
	}

	void Gui::showPointer()
	{
		mPointerManager->show();
	}

	bool Gui::isShowPointer()
	{
		return mPointerManager->isShow();
	}

	void Gui::setActiveViewport(Ogre::ushort _num)
	{
		if (_num == mActiveViewport) return;
		MYGUI_ASSERT(mWindow, "Gui is not initialised.");
		MYGUI_ASSERT(mWindow->getNumViewports() >= _num, "index out of range");
		mActiveViewport = _num;
		// �������� ����������
		windowResized(mWindow);
	}

	void Gui::setSceneManager(Ogre::SceneManager * _scene)
	{
		mLayerManager->setSceneManager(_scene);
	}

	void Gui::injectFrameEntered(Ogre::Real timeSinceLastFrame)
	{
		eventFrameStart(timeSinceLastFrame);
	}

	void Gui::_unlinkWidget(WidgetPtr _widget)
	{
		eventFrameStart.clear(_widget);
	}

	const std::string& Gui::getResourceGroup()
	{
		return mResourceManager->getResourceGroup();
	}

} // namespace MyGUI
