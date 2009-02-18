/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
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

	MYGUI_INSTANCE_IMPLEMENT(Gui);

	void Gui::initialise(Ogre::RenderWindow* _window, const std::string& _core, const Ogre::String & _group, Ogre::String _logFileName)
	{
		// самый первый лог
		LogManager::registerSection(MYGUI_LOG_SECTION, _logFileName);

		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");

		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);
		MYGUI_LOG(Info, "* MyGUI version "
			<< MYGUI_VERSION_MAJOR << "."
			<< MYGUI_VERSION_MINOR << "."
			<< MYGUI_VERSION_PATCH);

		// дефолтный вьюпорт
		mActiveViewport = 0;
		// сохраняем окно и размеры
		mWindow = _window;
		if (mWindow != nullptr) {
			mViewSize.set(mWindow->getViewport(mActiveViewport)->getActualWidth(), mWindow->getViewport(mActiveViewport)->getActualHeight());
		}

		MYGUI_LOG(Info, "Viewport : " << mViewSize.print());

		// создаем и инициализируем синглтоны
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

		// подписываемся на изменение размеров окна и сразу оповещаем
		if (mWindow != nullptr) {
			Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
			windowResized(mWindow);
		}

		// загружаем дефолтные настройки если надо
		if ( _core.empty() == false ) mResourceManager->load(_core, mResourceManager->getResourceGroup());

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void Gui::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		// скрываем сразу дебагеры
		mInputManager->setShowFocus(false);

		_destroyAllChildWidget();

		// отписываемся
		Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);

		// деинициализируем и удаляем синглтоны
		mPointerManager->shutdown();
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

		WidgetManager::getInstance().unregisterUnlinker(this);
		mWidgetManager->shutdown();

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

		// самый последний лог
		LogManager::shutdown();

		mIsInitialise = false;
	}

	bool Gui::injectMouseMove( int _absx, int _absy, int _absz) { return mInputManager->injectMouseMove(_absx, _absy, _absz); }
	bool Gui::injectMousePress( int _absx, int _absy, MouseButton _id ) { return mInputManager->injectMousePress(_absx, _absy, _id); }
	bool Gui::injectMouseRelease( int _absx, int _absy, MouseButton _id ) { return mInputManager->injectMouseRelease(_absx, _absy, _id); }

	bool Gui::injectKeyPress(KeyCode _key, Char _text) { return mInputManager->injectKeyPress(_key, _text); }
	bool Gui::injectKeyRelease(KeyCode _key) { return mInputManager->injectKeyRelease(_key); }


	WidgetPtr Gui::baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_style, _type, _skin, _coord, _align, nullptr, nullptr, this, _name);
		mWidgetChild.push_back(widget);
		// присоединяем виджет с уровню
		if (!_layer.empty()) LayerManager::getInstance().attachToLayerKeeper(_layer, widget);
		return widget;
	}

	WidgetPtr Gui::findWidgetT(const std::string& _name, bool _throw)
	{
		return mWidgetManager->findWidgetT(_name, _throw);
	}

	// удяляет неудачника
	void Gui::_destroyChildWidget(WidgetPtr _widget)
	{
		MYGUI_ASSERT(nullptr != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end()) {

			// сохраняем указатель
			MyGUI::WidgetPtr widget = *iter;

			// удаляем из списка
			*iter = mWidgetChild.back();
			mWidgetChild.pop_back();

			// отписываем от всех
			mWidgetManager->unlinkFromUnlinkers(_widget);

			// непосредственное удаление
			_deleteWidget(widget);
		}
		else MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
	}

	// удаляет всех детей
	void Gui::_destroyAllChildWidget()
	{
		while (false == mWidgetChild.empty()) {

			// сразу себя отписывем, иначе вложенной удаление убивает все
			WidgetPtr widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			//widget->detachWidget();

			// отписываем от всех
			mWidgetManager->unlinkFromUnlinkers(widget);

			// и сами удалим, так как его больше в списке нет
			_deleteWidget(widget);
		}
	}

	bool Gui::load(const std::string & _file, const std::string & _group)
	{
		return mResourceManager->load(_file, _group);
	}

	// для оповещений об изменении окна рендера
	void Gui::windowResized(Ogre::RenderWindow* rw)
	{
		IntSize oldViewSize = mViewSize;

		Ogre::Viewport * port = rw->getViewport(mActiveViewport);
		mViewSize.set(port->getActualWidth(), port->getActualHeight());
		mLayerManager->_windowResized(mViewSize);

		// выравниваем рутовые окна
		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			_alignWidget((*iter), oldViewSize, mViewSize);
		}
	}

	void Gui::destroyWidget(WidgetPtr _widget)
	{
		mWidgetManager->destroyWidget(_widget);
	}

	void Gui::destroyWidgets(VectorWidgetPtr & _widgets)
	{
		mWidgetManager->destroyWidgets(_widgets);
	}

	void Gui::destroyWidgets(EnumeratorWidgetPtr & _widgets)
	{
		mWidgetManager->destroyWidgets(_widgets);
	}

	void Gui::_alignWidget(WidgetPtr _widget, const IntSize& _old, const IntSize& _new)
	{
		if (nullptr == _widget) return;

		Align align = _widget->getAlign();
		if (align.isDefault()) return;

		IntCoord coord = _widget->getCoord();

		// первоначальное выравнивание
		if (align.isHStretch()) {
			// растягиваем
			coord.width += _new.width - _old.width;
		}
		else if (align.isRight()) {
			// двигаем по правому краю
			coord.left += _new.width - _old.width;
		}
		else if (align.isHCenter()) {
			// выравнивание по горизонтали без растяжения
			coord.left = (_new.width - coord.width) / 2;
		}

		if (align.isVStretch()) {
			// растягиваем
			coord.height += _new.height - _old.height;
		}
		else if (align.isBottom()) {
			// двигаем по нижнему краю
			coord.top += _new.height - _old.height;
		}
		else if (align.isVCenter()) {
			// выравнивание по вертикали без растяжения
			coord.top = (_new.height - coord.height) / 2;
		}

		_widget->setCoord(coord);
	}

	void Gui::hidePointer()
	{
		mPointerManager->setVisible(false);
	}

	void Gui::showPointer()
	{
		mPointerManager->setVisible(true);
	}

	bool Gui::isShowPointer()
	{
		return mPointerManager->isVisible();
	}

	void Gui::setActiveViewport(Ogre::ushort _num)
	{
		if (_num == mActiveViewport) return;
		MYGUI_ASSERT(mWindow, "Gui is not initialised.");
		MYGUI_ASSERT(mWindow->getNumViewports() >= _num, "index out of range");
		mActiveViewport = _num;
		// рассылка обновлений
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

	void Gui::_linkChildWidget(WidgetPtr _widget)
	{
		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter == mWidgetChild.end(), "widget already exist");
		mWidgetChild.push_back(_widget);
	}

	void Gui::_unlinkChildWidget(WidgetPtr _widget)
	{
		VectorWidgetPtr::iterator iter = std::remove(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter != mWidgetChild.end(), "widget not found");
		mWidgetChild.erase(iter);
	}

} // namespace MyGUI
