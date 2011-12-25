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
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Widget.h"
#include "MyGUI_FactoryManager.h"

#include "MyGUI_Button.h"
#include "MyGUI_Canvas.h"
#include "MyGUI_ComboBox.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_EditBox.h"
#include "MyGUI_ItemBox.h"
#include "MyGUI_ListBox.h"
#include "MyGUI_MenuBar.h"
#include "MyGUI_MenuControl.h"
#include "MyGUI_MenuItem.h"
#include "MyGUI_MultiListBox.h"
#include "MyGUI_MultiListItem.h"
#include "MyGUI_PopupMenu.h"
#include "MyGUI_ProgressBar.h"
#include "MyGUI_ScrollBar.h"
#include "MyGUI_ScrollView.h"
#include "MyGUI_ImageBox.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_TabControl.h"
#include "MyGUI_TabItem.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Window.h"

#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	template <> WidgetManager* Singleton<WidgetManager>::msInstance = nullptr;
	template <> const char* Singleton<WidgetManager>::mClassTypeName("WidgetManager");

	WidgetManager::WidgetManager() :
		mIsInitialise(false)
	{
	}

	void WidgetManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		FactoryManager& factory = FactoryManager::getInstance();

		factory.registerFactory<Button>("Widget");
		factory.registerFactory<Canvas>("Widget");
		factory.registerFactory<ComboBox>("Widget");
		factory.registerFactory<DDContainer>("Widget");
		factory.registerFactory<EditBox>("Widget");
		factory.registerFactory<ItemBox>("Widget");
		factory.registerFactory<ListBox>("Widget");
		factory.registerFactory<MenuBar>("Widget");
		factory.registerFactory<MenuControl>("Widget");
		factory.registerFactory<MenuItem>("Widget");
		factory.registerFactory<MultiListBox>("Widget");
		factory.registerFactory<MultiListItem>("Widget");
		factory.registerFactory<PopupMenu>("Widget");
		factory.registerFactory<ProgressBar>("Widget");
		factory.registerFactory<ScrollBar>("Widget");
		factory.registerFactory<ScrollView>("Widget");
		factory.registerFactory<ImageBox>("Widget");
		factory.registerFactory<TextBox>("Widget");
		factory.registerFactory<TabControl>("Widget");
		factory.registerFactory<TabItem>("Widget");
		factory.registerFactory<Widget>("Widget");
		factory.registerFactory<Window>("Widget");

		BackwardCompatibility::registerWidgetTypes();

		Gui::getInstance().eventFrameStart += newDelegate(this, &WidgetManager::notifyEventFrameStart);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void WidgetManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		Gui::getInstance().eventFrameStart -= newDelegate(this, &WidgetManager::notifyEventFrameStart);
		_deleteDelayWidgets();

		mVectorIUnlinkWidget.clear();

		FactoryManager::getInstance().unregisterFactory("Widget");

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	Widget* WidgetManager::createWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Widget* _parent, ICroppedRectangle* _cropeedParent, const std::string& _name)
	{
		IObject* object = FactoryManager::getInstance().createObject("Widget", _type);
		if (object != nullptr)
		{
			Widget* widget = object->castType<Widget>();
			widget->_initialise(_style, _coord, _skin, _parent, _cropeedParent, _name);

			return widget;
		}

		MYGUI_EXCEPT("factory '" << _type << "' not found");
	}

	void WidgetManager::destroyWidget(Widget* _widget)
	{
		Gui::getInstance().destroyWidget(_widget);
	}

	void WidgetManager::destroyWidgets(const VectorWidgetPtr& _widgets)
	{
		Gui::getInstance().destroyWidgets(_widgets);
	}

	void WidgetManager::destroyWidgets(EnumeratorWidgetPtr _widgets)
	{
		Gui::getInstance().destroyWidgets(_widgets);
	}

	void WidgetManager::registerUnlinker(IUnlinkWidget* _unlink)
	{
		unregisterUnlinker(_unlink);
		mVectorIUnlinkWidget.push_back(_unlink);
	}

	void WidgetManager::unregisterUnlinker(IUnlinkWidget* _unlink)
	{
		VectorIUnlinkWidget::iterator iter = std::remove(mVectorIUnlinkWidget.begin(), mVectorIUnlinkWidget.end(), _unlink);
		if (iter != mVectorIUnlinkWidget.end())
			mVectorIUnlinkWidget.erase(iter);
	}

	void WidgetManager::unlinkFromUnlinkers(Widget* _widget)
	{
		for (VectorIUnlinkWidget::iterator iter = mVectorIUnlinkWidget.begin(); iter != mVectorIUnlinkWidget.end(); ++iter)
		{
			(*iter)->_unlinkWidget(_widget);
		}
	}

	bool WidgetManager::isFactoryExist(const std::string& _type)
	{
		if (FactoryManager::getInstance().isFactoryExist("Widget", _type))
		{
			return true;
		}

		return false;
	}

	void WidgetManager::notifyEventFrameStart(float _time)
	{
		_deleteDelayWidgets();
	}

	void WidgetManager::_deleteWidget(Widget* _widget)
	{
		_widget->_shutdown();

		for (VectorWidgetPtr::iterator entry = mDestroyWidgets.begin(); entry != mDestroyWidgets.end(); ++entry)
		{
			/*if ((*entry) == _widget)
				return;*/
			MYGUI_ASSERT((*entry) != _widget, "double delete widget");
		}

		mDestroyWidgets.push_back(_widget);
	}

	void WidgetManager::_deleteDelayWidgets()
	{
		if (!mDestroyWidgets.empty())
		{
			for (VectorWidgetPtr::iterator entry = mDestroyWidgets.begin(); entry != mDestroyWidgets.end(); ++entry)
				delete (*entry);
			mDestroyWidgets.clear();
		}
	}

} // namespace MyGUI
