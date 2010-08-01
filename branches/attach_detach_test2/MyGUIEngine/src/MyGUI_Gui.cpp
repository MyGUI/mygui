/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
/*
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
#include "MyGUI_LanguageManager.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_ToolTipManager.h"

namespace MyGUI
{

	template <> const char* Singleton<Gui>::mClassTypeName("Gui");

	Gui::Gui() :
		mInputManager(nullptr),
		mSubWidgetManager(nullptr),
		mLayerManager(nullptr),
		mSkinManager(nullptr),
		mWidgetManager(nullptr),
		mFontManager(nullptr),
		mControllerManager(nullptr),
		mPointerManager(nullptr),
		mClipboardManager(nullptr),
		mLayoutManager(nullptr),
		mDynLibManager(nullptr),
		mPluginManager(nullptr),
		mLanguageManager(nullptr),
		mResourceManager(nullptr),
		mFactoryManager(nullptr),
		mToolTipManager(nullptr),
		mIsInitialise(false)
	{
	}

	void Gui::initialise(const std::string& _core, const std::string& _logFileName)
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_LOG(Info, "* MyGUI version "
			<< MYGUI_VERSION_MAJOR << "."
			<< MYGUI_VERSION_MINOR << "."
			<< MYGUI_VERSION_PATCH);

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
		mLanguageManager = new LanguageManager();
		mFactoryManager = new FactoryManager();
		mToolTipManager = new ToolTipManager();

		mResourceManager->initialise();
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
		mLanguageManager->initialise();
		mFactoryManager->initialise();
		mToolTipManager->initialise();

		WidgetManager::getInstance().registerUnlinker(this);

		// загружаем дефолтные настройки если надо
		if ( _core.empty() == false ) mResourceManager->load(_core);

		mViewSize = RenderManager::getInstance().getViewSize();
		_resizeWindow(mViewSize);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void Gui::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllChilds();

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
		mLanguageManager->shutdown();
		mResourceManager->shutdown();
		mFactoryManager->shutdown();
		mToolTipManager->shutdown();

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
		delete mLanguageManager;
		delete mResourceManager;
		delete mFactoryManager;
		delete mToolTipManager;

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	Widget* Gui::findWidgetT(const std::string& _name, bool _throw)
	{
		for (VectorWidgetPtr::iterator iter = mChilds.begin(); iter!=mChilds.end(); ++iter)
		{
			Widget* widget = (*iter)->findWidget(_name);
			if (widget != nullptr) return widget;
		}
		MYGUI_ASSERT(!_throw, "Widget '" << _name << "' not found");
		return nullptr;
	}

	void Gui::destroyAllChilds()
	{
		while (!mChilds.empty())
			destroyChild(mChilds.front());
	}

	void Gui::destroyChild(Widget* _widget)
	{
		detachChild(_widget);

		WidgetManager::getInstance()._deleteWidget2(_widget);
	}

	void Gui::_injectFrameEntered(float _time)
	{
		eventFrameStart(_time);
	}

	void Gui::_unlinkWidget(Widget* _widget)
	{
		eventFrameStart.clear(_widget);
	}

	void Gui::_resizeWindow(const IntSize& _size)
	{
		IntSize oldViewSize = mViewSize;
		mViewSize = _size;

		// выравниваем рутовые окна
		for (VectorWidgetPtr::iterator iter = mChilds.begin(); iter!=mChilds.end(); ++iter)
			(*iter)->_setAlign(oldViewSize);
	}

#ifndef MYGUI_DONT_USE_OBSOLETE

	bool Gui::injectMouseMove( int _absx, int _absy, int _absz)
	{
		return mInputManager->injectMouseMove(_absx, _absy, _absz);
	}

	bool Gui::injectMousePress( int _absx, int _absy, MouseButton _id )
	{
		return mInputManager->injectMousePress(_absx, _absy, _id);
	}

	bool Gui::injectMouseRelease( int _absx, int _absy, MouseButton _id )
	{
		return mInputManager->injectMouseRelease(_absx, _absy, _id);
	}

	bool Gui::injectKeyPress(KeyCode _key, Char _text)
	{
		return mInputManager->injectKeyPress(_key, _text);
	}

	bool Gui::injectKeyRelease(KeyCode _key)
	{
		return mInputManager->injectKeyRelease(_key);
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

	const IntSize& Gui::getViewSize() const
	{
		return mViewSize;
	}

	bool Gui::load(const std::string& _file)
	{
		return mResourceManager->load(_file);
	}

	int Gui::getViewWidth()
	{
		return mViewSize.width;
	}

	int Gui::getViewHeight()
	{
		return mViewSize.height;
	}

	void Gui::setVisiblePointer(bool _value)
	{
		mPointerManager->setVisible(_value);
	}

	bool Gui::isVisiblePointer()
	{
		return mPointerManager->isVisible();
	}

#endif // MYGUI_DONT_USE_OBSOLETE

	void Gui::detachChild(Widget* _widget)
	{
		MYGUI_ASSERT(nullptr != _widget, "invalid widget pointer");
		MYGUI_ASSERT(_widget->getAttached(), "already detached");

		VectorWidgetPtr::iterator iter = std::find(mChilds.begin(), mChilds.end(), _widget);
		if (iter != mChilds.end())
		{
			_widget->_destroySkin();

			_widget->_setParent(nullptr);
			_widget->_setWidgetContainer(nullptr);
			mChilds.erase(iter);

			_widget->_createSkin();
		}
		else
		{
			MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
		}
	}

	void Gui::attachChild(Widget* _widget)
	{
		MYGUI_ASSERT(nullptr != _widget, "invalid widget pointer");
		MYGUI_ASSERT(!_widget->getAttached(), "already attached");

		_widget->_destroySkin();

		_widget->_setParent(nullptr);
		_widget->_setWidgetContainer(this);
		mChilds.push_back(_widget);

		_widget->_createSkin();
	}

	Widget* Gui::createWidgetImpl(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		Widget* widget = WidgetManager::getInstance().createWidget(_type);

		widget->setWidgetStyle(_style);
		widget->setAlign(_align);
		widget->setName(_name);
		widget->setCoord(_coord);
		widget->setSkinName(_skin);
		widget->setLayerName(_layer);

		attachChild(widget);

		return widget;
	}

} // namespace MyGUI
