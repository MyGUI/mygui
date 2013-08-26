/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Widget.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Gui.h"

#include "MyGUI_ResourceManualPointer.h"
#include "MyGUI_ResourceImageSetPointer.h"

namespace MyGUI
{

	template <> PointerManager* Singleton<PointerManager>::msInstance = nullptr;
	template <> const char* Singleton<PointerManager>::mClassTypeName = "PointerManager";

	PointerManager::PointerManager() :
		mVisible(false),
		mWidgetOwner(nullptr),
		mMousePointer(nullptr),
		mPointer(nullptr),
		mIsInitialise(false),
		mXmlPointerTagName("Pointer"),
		mXmlPropertyTagName("Property"),
		mXmlDefaultPointerValue("Default")
	{
	}

	void PointerManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		Gui::getInstance().eventFrameStart += newDelegate(this, &PointerManager::notifyFrameStart);
		InputManager::getInstance().eventChangeMouseFocus += newDelegate(this, &PointerManager::notifyChangeMouseFocus);
		WidgetManager::getInstance().registerUnlinker(this);

		ResourceManager::getInstance().registerLoadXmlDelegate(mXmlPointerTagName) = newDelegate(this, &PointerManager::_load);

		std::string resourceCategory = ResourceManager::getInstance().getCategoryName();
		FactoryManager::getInstance().registerFactory<ResourceManualPointer>(resourceCategory);
		FactoryManager::getInstance().registerFactory<ResourceImageSetPointer>(resourceCategory);

		mPointer = nullptr;
		mMousePointer = nullptr;
		mWidgetOwner = nullptr;
		mVisible = true;

		mSkinName = "ImageBox";

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void PointerManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		InputManager::getInstance().eventChangeMouseFocus -= newDelegate(this, &PointerManager::notifyChangeMouseFocus);
		Gui::getInstance().eventFrameStart -= newDelegate(this, &PointerManager::notifyFrameStart);

		std::string resourceCategory = ResourceManager::getInstance().getCategoryName();
		FactoryManager::getInstance().unregisterFactory<ResourceManualPointer>(resourceCategory);
		FactoryManager::getInstance().unregisterFactory<ResourceImageSetPointer>(resourceCategory);

		// удаляем все виджеты
		_destroyAllChildWidget();

		mWidgetOwner = nullptr;

		WidgetManager::getInstance().unregisterUnlinker(this);
		ResourceManager::getInstance().unregisterLoadXmlDelegate(mXmlPointerTagName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void PointerManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
#ifndef MYGUI_DONT_USE_OBSOLETE
		loadOldPointerFormat(_node, _file, _version, mXmlPointerTagName);
#endif // MYGUI_DONT_USE_OBSOLETE

		xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == mXmlPropertyTagName)
			{
				const std::string& key = node->findAttribute("key");
				const std::string& value = node->findAttribute("value");
				if (key == "Default")
					setDefaultPointer(value);
				else if (key == "Layer")
					setLayerName(value);
				else if (key == "Skin")
					mSkinName = value;
			}
		}
	}

	void PointerManager::notifyFrameStart(float _time)
	{
		mPoint = InputManager::getInstance().getMousePosition();
		if (mOldPoint != mPoint)
		{
			mOldPoint = mPoint;

			if (nullptr != mMousePointer && mPointer != nullptr)
				mPointer->setPosition(mMousePointer, mPoint);
		}
	}

	void PointerManager::setVisible(bool _visible)
	{
		if (nullptr != mMousePointer) mMousePointer->setVisible(_visible);
		mVisible = _visible;
	}

	void PointerManager::setPointer(const std::string& _name, Widget* _owner)
	{
		if (nullptr == mMousePointer)
			return;

		IResource* result = getByName(_name);
		if (result == nullptr)
		{
			mPointer = nullptr;
			mMousePointer->setVisible(false);
			return;
		}

		mMousePointer->setVisible(mVisible);
		mPointer = result->castType<IPointer>();
		mPointer->setImage(mMousePointer);
		mPointer->setPosition(mMousePointer, mPoint);

		mWidgetOwner = _owner;
	}

	void PointerManager::_unlinkWidget(Widget* _widget)
	{
		if (_widget == mWidgetOwner) setPointer(mDefaultName, nullptr);
		else if (_widget == mMousePointer) mMousePointer = nullptr;
	}

	void PointerManager::resetToDefaultPointer()
	{
		setPointer(mDefaultName, nullptr);
	}

	// создает виджет
	Widget* PointerManager::baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		Widget* widget = WidgetManager::getInstance().createWidget(_style, _type, _skin, _coord, /*_align, */nullptr, nullptr, /*this, */_name);
		mWidgetChild.push_back(widget);

		widget->setAlign(_align);

		// присоединяем виджет с уровню
		if (!_layer.empty())
			LayerManager::getInstance().attachToLayerNode(_layer, widget);
		return widget;
	}

	// удяляет неудачника
	void PointerManager::_destroyChildWidget(Widget* _widget)
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
			WidgetManager::getInstance().unlinkFromUnlinkers(_widget);

			// непосредственное удаление
			WidgetManager::getInstance()._deleteWidget(widget);
		}
		else
		{
			MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
		}
	}

	// удаляет всех детей
	void PointerManager::_destroyAllChildWidget()
	{
		WidgetManager& manager = WidgetManager::getInstance();
		while (!mWidgetChild.empty())
		{
			// сразу себя отписывем, иначе вложенной удаление убивает все
			Widget* widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			// отписываем от всех
			manager.unlinkFromUnlinkers(widget);

			// и сами удалим, так как его больше в списке нет
			WidgetManager::getInstance()._deleteWidget(widget);
		}
	}

	void PointerManager::setDefaultPointer(const std::string& _value)
	{
		Update();

		mDefaultName = _value;
		setPointer(mDefaultName, nullptr);
	}

	void PointerManager::setLayerName(const std::string& _value)
	{
		Update();

		mLayerName = _value;
		if (LayerManager::getInstance().isExist(_value))
			LayerManager::getInstance().attachToLayerNode(mLayerName, mMousePointer);
	}

	void PointerManager::Update()
	{
		if (mMousePointer == nullptr)
			mMousePointer = static_cast<ImageBox*>(baseCreateWidget(WidgetStyle::Overlapped, ImageBox::getClassTypeName(), mSkinName, IntCoord(), Align::Default, "", ""));
	}

	IPointer* PointerManager::getByName(const std::string& _name) const
	{
		IResource* result = nullptr;
		if (!_name.empty() && _name != mXmlDefaultPointerValue)
			result = ResourceManager::getInstance().getByName(_name, false);

		if (result == nullptr)
			result = ResourceManager::getInstance().getByName(mDefaultName, false);

		return result ? result->castType<IPointer>(false) : nullptr;
	}

	void PointerManager::notifyChangeMouseFocus(Widget* _widget)
	{
		std::string pointer = (_widget == nullptr || !_widget->getInheritedEnabled()) ? "" : _widget->getPointer();
		if (pointer != mCurrentMousePointer)
		{
			mCurrentMousePointer = pointer;
			if (mCurrentMousePointer.empty())
			{
				resetToDefaultPointer();
				eventChangeMousePointer(mDefaultName);
			}
			else
			{
				setPointer(mCurrentMousePointer, _widget);
				eventChangeMousePointer(mCurrentMousePointer);
			}
		}
	}

	void PointerManager::setPointer(const std::string& _name)
	{
		setPointer(_name, nullptr);
	}

	bool PointerManager::isVisible() const
	{
		return mVisible;
	}

	const std::string& PointerManager::getDefaultPointer() const
	{
		return mDefaultName;
	}

	const std::string& PointerManager::getLayerName() const
	{
		return mLayerName;
	}

} // namespace MyGUI
