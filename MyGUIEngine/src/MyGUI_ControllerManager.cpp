/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Gui.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_FactoryManager.h"

#include "MyGUI_ControllerEdgeHide.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_ControllerPosition.h"
#include "MyGUI_ControllerRepeatClick.h"

namespace MyGUI
{

	MYGUI_SINGLETON_DEFINITION(ControllerManager);

	ControllerManager::ControllerManager() :
		mCategoryName("Controller"),
		mSingletonHolder(this)
	{
	}

	void ControllerManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		WidgetManager::getInstance().registerUnlinker(this);

		FactoryManager::getInstance().registerFactory<ControllerEdgeHide>(mCategoryName);
		FactoryManager::getInstance().registerFactory<ControllerFadeAlpha>(mCategoryName);
		FactoryManager::getInstance().registerFactory<ControllerPosition>(mCategoryName);
		FactoryManager::getInstance().registerFactory<ControllerRepeatClick>(mCategoryName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void ControllerManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		FactoryManager::getInstance().unregisterFactory<ControllerEdgeHide>(mCategoryName);
		FactoryManager::getInstance().unregisterFactory<ControllerFadeAlpha>(mCategoryName);
		FactoryManager::getInstance().unregisterFactory<ControllerPosition>(mCategoryName);
		FactoryManager::getInstance().unregisterFactory<ControllerRepeatClick>(mCategoryName);

		WidgetManager::getInstance().unregisterUnlinker(this);
		clear();

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void ControllerManager::clear()
	{
		for (auto& iter : mListItem)
		{
			delete iter.second;
		}
		mListItem.clear();
	}

	ControllerItem* ControllerManager::createItem(std::string_view _type)
	{
		IObject* object = FactoryManager::getInstance().createObject(mCategoryName, _type);
		return object == nullptr ? nullptr : object->castType<ControllerItem>();
	}

	void ControllerManager::addItem(Widget* _widget, ControllerItem* _item)
	{
		_item->prepareItem(_widget);

		for (auto& iter : mListItem)
		{
			// already in the list
			if (iter.first == _widget)
			{
				if (iter.second->getTypeName() == _item->getTypeName())
				{
					delete iter.second;
					iter.second = _item;
					return;
				}
			}
		}

		// subscribe to events only for first item
		if (mListItem.empty())
			Gui::getInstance().eventFrameStart += newDelegate(this, &ControllerManager::frameEntered);

		mListItem.emplace_back(_widget, _item);
	}

	void ControllerManager::removeItem(Widget* _widget)
	{
		// replace with nullptr instead of removing, will  be deleted in frameEntered
		for (auto& iter : mListItem)
		{
			if (iter.first == _widget)
				iter.first = nullptr;
		}
	}

	void ControllerManager::_unlinkWidget(Widget* _widget)
	{
		removeItem(_widget);
	}

	void ControllerManager::frameEntered(float _time)
	{
		for (ListControllerItem::iterator iter = mListItem.begin(); iter != mListItem.end(); /*added in body*/)
		{
			if (nullptr == (*iter).first)
			{
				delete (*iter).second;
				iter = mListItem.erase(iter);
				continue;
			}

			if ((*iter).second->addTime((*iter).first, _time))
			{
				++iter;
				continue;
			}

			// will be removed in next iteration
			(*iter).first = nullptr;
		}

		if (mListItem.empty())
			Gui::getInstance().eventFrameStart -= newDelegate(this, &ControllerManager::frameEntered);
	}

	const std::string& ControllerManager::getCategoryName() const
	{
		return mCategoryName;
	}

} // namespace MyGUI
