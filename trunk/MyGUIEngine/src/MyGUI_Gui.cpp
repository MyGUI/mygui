/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
#include "MyGUI_TextureUtility.h"

namespace MyGUI
{

	template <> Gui* Singleton<Gui>::msInstance = nullptr;
	template <> const char* Singleton<Gui>::mClassTypeName = "Gui";

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

	void Gui::initialise(const std::string& _core)
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

#ifdef MYGUI_SVN_REVISION
		MYGUI_LOG(Info, "* MyGUI version "
			<< MYGUI_VERSION_MAJOR << "."
			<< MYGUI_VERSION_MINOR << "."
			<< MYGUI_VERSION_PATCH << "."
			<< MYGUI_SVN_REVISION);
#else
		MYGUI_LOG(Info, "* MyGUI version "
			<< MYGUI_VERSION_MAJOR << "."
			<< MYGUI_VERSION_MINOR << "."
			<< MYGUI_VERSION_PATCH);
#endif

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
		if (!_core.empty())
			mResourceManager->load(_core);

		BackwardCompatibility::initialise();

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

#ifndef MYGUI_DONT_USE_OBSOLETE
	void Gui::initialise(const std::string& _core, const std::string& _logFileName)
	{
		initialise(_core);
	}
#endif // MYGUI_DONT_USE_OBSOLETE

	void Gui::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		BackwardCompatibility::shutdown();

		_destroyAllChildWidget();

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

		// сбрасываем кеш
		texture_utility::getTextureSize("", false);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	Widget* Gui::baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		Widget* widget = WidgetManager::getInstance().createWidget(_style, _type, _skin, _coord, /*_align, */nullptr, nullptr, _name);
		mWidgetChild.push_back(widget);

		widget->setAlign(_align);

		// присоединяем виджет с уровню
		if (!_layer.empty())
			LayerManager::getInstance().attachToLayerNode(_layer, widget);
		return widget;
	}

	Widget* Gui::findWidgetT(const std::string& _name, bool _throw)
	{
		for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter)
		{
			Widget* widget = (*iter)->findWidget(_name);
			if (widget != nullptr) return widget;
		}
		MYGUI_ASSERT(!_throw, "Widget '" << _name << "' not found");
		return nullptr;
	}

	// удяляет неудачника
	void Gui::_destroyChildWidget(Widget* _widget)
	{
		MYGUI_ASSERT(nullptr != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end())
		{
			// сохраняем указатель
			MyGUI::Widget* widget = *iter;

			// удаляем из списка
			mWidgetChild.erase(iter);

			// отписываем от всех
			mWidgetManager->unlinkFromUnlinkers(_widget);

			// непосредственное удаление
			WidgetManager::getInstance()._deleteWidget(widget);
		}
		else
		{
			MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
		}
	}

	// удаляет всех детей
	void Gui::_destroyAllChildWidget()
	{
		while (!mWidgetChild.empty())
		{
			// сразу себя отписывем, иначе вложенной удаление убивает все
			Widget* widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			// отписываем от всех
			mWidgetManager->unlinkFromUnlinkers(widget);

			// и сами удалим, так как его больше в списке нет
			WidgetManager::getInstance()._deleteWidget(widget);
		}
	}

	void Gui::destroyWidget(Widget* _widget)
	{
		Widget* parent = _widget->getParent();
		if (parent != nullptr)
			parent->_destroyChildWidget(_widget);
		else
			_destroyChildWidget(_widget);
	}

	void Gui::destroyWidgets(const VectorWidgetPtr& _widgets)
	{
		for (VectorWidgetPtr::const_iterator iter = _widgets.begin(); iter != _widgets.end(); ++iter)
			destroyWidget(*iter);
	}

	void Gui::destroyWidgets(EnumeratorWidgetPtr& _widgets)
	{
		VectorWidgetPtr widgets;
		while (_widgets.next())
			widgets.push_back(_widgets.current());
		destroyWidgets(widgets);
	}

	void Gui::_unlinkWidget(Widget* _widget)
	{
		eventFrameStart.clear(_widget);
	}

	void Gui::_linkChildWidget(Widget* _widget)
	{
		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter == mWidgetChild.end(), "widget already exist");
		mWidgetChild.push_back(_widget);
	}

	void Gui::_unlinkChildWidget(Widget* _widget)
	{
		VectorWidgetPtr::iterator iter = std::remove(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		MYGUI_ASSERT(iter != mWidgetChild.end(), "widget not found");
		mWidgetChild.erase(iter);
	}

	Widget* Gui::createWidgetT(const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		return baseCreateWidget(WidgetStyle::Overlapped, _type, _skin, _coord, _align, _layer, _name);
	}
	/** See Gui::createWidgetT */
	Widget* Gui::createWidgetT(const std::string& _type, const std::string& _skin, int _left, int _top, int _width, int _height, Align _align, const std::string& _layer, const std::string& _name)
	{
		return createWidgetT(_type, _skin, IntCoord(_left, _top, _width, _height), _align, _layer, _name);
	}
	/** Create widget using coordinates relative to parent widget. see Gui::createWidgetT */
	Widget* Gui::createWidgetRealT(const std::string& _type, const std::string& _skin, const FloatCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		IntSize size = RenderManager::getInstance().getViewSize();
		return createWidgetT(_type, _skin, IntCoord((int)(_coord.left * size.width), (int)(_coord.top * size.height), (int)(_coord.width * size.width), (int)(_coord.height * size.height)), _align, _layer, _name);
	}
	/** Create widget using coordinates relative to parent. see Gui::createWidgetT */
	Widget* Gui::createWidgetRealT(const std::string& _type, const std::string& _skin, float _left, float _top, float _width, float _height, Align _align, const std::string& _layer, const std::string& _name)
	{
		IntSize size = RenderManager::getInstance().getViewSize();
		return createWidgetT(_type, _skin, IntCoord((int)(_left * size.width), (int)(_top * size.height), (int)(_width * size.width), (int)(_height * size.height)), _align, _layer, _name);
	}

	Widget* Gui::findWidgetT(const std::string& _name, const std::string& _prefix, bool _throw)
	{
		return findWidgetT(_prefix + _name, _throw);
	}

	void Gui::destroyChildWidget(Widget* _widget)
	{
		_destroyChildWidget(_widget);
	}

	void Gui::destroyAllChildWidget()
	{
		_destroyAllChildWidget();
	}

	EnumeratorWidgetPtr Gui::getEnumerator() const
	{
		return EnumeratorWidgetPtr(mWidgetChild);
	}

	void Gui::frameEvent(float _time)
	{
		eventFrameStart(_time);
	}

} // namespace MyGUI
