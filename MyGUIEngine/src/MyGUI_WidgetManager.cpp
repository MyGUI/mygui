/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
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
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_IWidgetCreator.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_FactoryManager.h"

#include "MyGUI_Button.h"
#include "MyGUI_Canvas.h"
#include "MyGUI_ComboBox.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_Edit.h"
#include "MyGUI_HScroll.h"
#include "MyGUI_ItemBox.h"
#include "MyGUI_List.h"
#include "MyGUI_ListBox.h"
#include "MyGUI_ListCtrl.h"
#include "MyGUI_MenuBar.h"
#include "MyGUI_MenuCtrl.h"
#include "MyGUI_MenuItem.h"
#include "MyGUI_Message.h"
#include "MyGUI_MultiList.h"
#include "MyGUI_PopupMenu.h"
#include "MyGUI_Progress.h"
#include "MyGUI_RenderBox.h"
#include "MyGUI_ScrollView.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_Tab.h"
#include "MyGUI_TabItem.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Window.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(WidgetManager);

	void WidgetManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		//registerUnlinker(this);

		FactoryManager& factory = FactoryManager::getInstance();

		factory.registryFactory<Button>("Widget");
		factory.registryFactory<Canvas>("Widget");
		factory.registryFactory<ComboBox>("Widget");
		factory.registryFactory<DDContainer>("Widget");
		factory.registryFactory<Edit>("Widget");
		factory.registryFactory<HScroll>("Widget");
		factory.registryFactory<ItemBox>("Widget");
		factory.registryFactory<List>("Widget");
		factory.registryFactory<ListBox>("Widget");
		factory.registryFactory<ListCtrl>("Widget");
		factory.registryFactory<MenuBar>("Widget");
		factory.registryFactory<MenuCtrl>("Widget");
		factory.registryFactory<MenuItem>("Widget");
		factory.registryFactory<Message>("Widget");
		factory.registryFactory<MultiList>("Widget");
		factory.registryFactory<PopupMenu>("Widget");
		factory.registryFactory<Progress>("Widget");
		factory.registryFactory<RenderBox>("Widget");
		factory.registryFactory<ScrollView>("Widget");
		factory.registryFactory<StaticImage>("Widget");
		factory.registryFactory<StaticText>("Widget");
		factory.registryFactory<Tab>("Widget");
		factory.registryFactory<TabItem>("Widget");
		factory.registryFactory<VScroll>("Widget");
		factory.registryFactory<Widget>("Widget");
		factory.registryFactory<Window>("Widget");

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void WidgetManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		//unregisterUnlinker(this);

		mFactoryList.clear();
		mDelegates.clear();
		mVectorIUnlinkWidget.clear();

		FactoryManager::getInstance().unregistryFactory("Widget");

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void WidgetManager::registerFactory(IWidgetFactory * _factory)
	{
		mFactoryList.insert(_factory);
		MYGUI_LOG(Info, "* Register widget factory '" << _factory->getTypeName() << "'");
	}

	void WidgetManager::unregisterFactory(IWidgetFactory * _factory)
	{
		SetWidgetFactory::iterator iter = mFactoryList.find(_factory);
		if (iter != mFactoryList.end()) mFactoryList.erase(iter);
		MYGUI_LOG(Info, "* Unregister widget factory '" << _factory->getTypeName() << "'");
	}

	WidgetPtr WidgetManager::createWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, WidgetPtr _parent, ICroppedRectangle * _cropeedParent, IWidgetCreator * _creator, const std::string& _name)
	{
		IObject* object = FactoryManager::getInstance().createObject("Widget", _type);
		if (object != nullptr)
		{
			WidgetPtr widget = object->castType<Widget>();
			ResourceSkin* skin = SkinManager::getInstance().getByName(_skin/*, false*/);
			/*if (skin == nullptr)
			{
				skin = SkinManager::getInstance().getByName("skin_Default");
			}*/
			widget->_initialise(_style, _coord, _align, skin, _parent, _cropeedParent, _creator, _name);

			return widget;
		}

		// старый вариант создания
		/*std::string name;
		if (false == _name.empty())
		{
			MapWidgetPtr::iterator iter = mWidgets.find(_name);
			MYGUI_ASSERT(iter == mWidgets.end(), "widget with name '" << _name << "' already exist");
			name = _name;
		}
		else
		{
			static long num = 0;
			name = utility::toString(num++, "_", _type);
		}*/

		for (SetWidgetFactory::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++)
		{
			if ( (*factory)->getTypeName() == _type)
			{
				WidgetPtr widget = (*factory)->createWidget(_style, _skin, _coord, _align, _parent, _cropeedParent, _creator, _name);
				//mWidgets[name] = widget;
				return widget;
			}
		}

		MYGUI_EXCEPT("factory '" << _type << "' not found");
		return nullptr;
	}

	WidgetPtr WidgetManager::findWidgetT(const std::string& _name, bool _throw)
	{
		return Gui::getInstance().findWidgetT(_name, _throw);

		/*MapWidgetPtr::iterator iter = mWidgets.find(_name);
		if (iter == mWidgets.end())
		{
			MYGUI_ASSERT(!_throw, "Widget '" << _name << "' not found");
			return nullptr;
		}
		return iter->second;*/
	}

	WidgetPtr WidgetManager::findWidgetT(const std::string& _name, const std::string& _prefix, bool _throw)
	{
		return Gui::getInstance().findWidgetT(_name, _prefix, _throw);
	}

	/*void WidgetManager::_unlinkWidget(WidgetPtr _widget)
	{
		if (_widget == nullptr) return;
		MapWidgetPtr::iterator iter = mWidgets.find(_widget->getName());
		if (iter != mWidgets.end()) mWidgets.erase(iter);
	}*/

	ParseDelegate& WidgetManager::registerDelegate(const std::string& _key)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		MYGUI_ASSERT(iter == mDelegates.end(), "delegate with name '" << _key << "' already exist");
		return (mDelegates[_key] = ParseDelegate());
	}

	void WidgetManager::unregisterDelegate(const std::string& _key)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter != mDelegates.end()) mDelegates.erase(iter);
	}

	void WidgetManager::parse(WidgetPtr _widget, const std::string &_key, const std::string &_value)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter == mDelegates.end())
		{
			//MYGUI_LOG(Error, "Unknown key '" << _key << "' with value '" << _value << "'");
			_widget->setProperty(_key, _value);
			return;
		}
		iter->second(_widget, _key, _value);
	}

	void WidgetManager::destroyWidget(WidgetPtr _widget)
	{
		// иначе возможен бесконечный цикл
		MYGUI_ASSERT(_widget != nullptr, "widget is deleted");

		// делегирует удаление отцу виджета
		IWidgetCreator * creator = _widget->_getIWidgetCreator();
		creator->_destroyChildWidget(_widget);
	}

	void WidgetManager::destroyWidgets(const VectorWidgetPtr& _widgets)
	{
		for (VectorWidgetPtr::const_iterator iter = _widgets.begin(); iter != _widgets.end(); ++iter)
		{
			destroyWidget(*iter);
		}
	}

	void WidgetManager::destroyWidgets(EnumeratorWidgetPtr _widgets)
	{
		VectorWidgetPtr widgets;
		while (_widgets.next())
		{
			widgets.push_back(_widgets.current());
		};
		destroyWidgets(widgets);
	}

	void WidgetManager::registerUnlinker(IUnlinkWidget * _unlink)
	{
		unregisterUnlinker(_unlink);
		mVectorIUnlinkWidget.push_back(_unlink);
	}

	void WidgetManager::unregisterUnlinker(IUnlinkWidget * _unlink)
	{
		for (size_t pos=0; pos<mVectorIUnlinkWidget.size(); pos++)
		{
			if (mVectorIUnlinkWidget[pos] == _unlink)
			{
				mVectorIUnlinkWidget[pos] = mVectorIUnlinkWidget[mVectorIUnlinkWidget.size()-1];
				mVectorIUnlinkWidget.pop_back();
				break;
			}
		}
	}

	void WidgetManager::unlinkFromUnlinkers(WidgetPtr _widget)
	{
		for (VectorIUnlinkWidget::iterator iter = mVectorIUnlinkWidget.begin(); iter!=mVectorIUnlinkWidget.end(); ++iter)
		{
			(*iter)->_unlinkWidget(_widget);
		}
		// вызывать последним, обнулится
		removeWidgetFromUnlink(_widget);
	}

	void WidgetManager::addWidgetToUnlink(WidgetPtr _widget)
	{
		if (_widget) mUnlinkWidgets.push_back(_widget);
	}

	void WidgetManager::removeWidgetFromUnlink(WidgetPtr& _widget)
	{
		VectorWidgetPtr::iterator iter = std::find(mUnlinkWidgets.begin(), mUnlinkWidgets.end(), _widget);
		if (iter != mUnlinkWidgets.end())
		{
			(*iter) = mUnlinkWidgets.back();
			mUnlinkWidgets.pop_back();
		}
		else
		{
			_widget = nullptr;
		}
	}

} // namespace MyGUI
