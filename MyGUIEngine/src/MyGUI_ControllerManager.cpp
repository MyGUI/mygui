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

	template <> ControllerManager* Singleton<ControllerManager>::msInstance = nullptr;
	template <> const char* Singleton<ControllerManager>::mClassTypeName = "ControllerManager";

	ControllerManager::ControllerManager() :
		mIsInitialise(false),
		mCategoryName("Controller")
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
		for (ListControllerItem::iterator iter = mListItem.begin(); iter != mListItem.end(); ++iter)
		{
			delete (*iter).second;
		}
		mListItem.clear();
	}

	ControllerItem* ControllerManager::createItem(const std::string& _type)
	{
		IObject* object = FactoryManager::getInstance().createObject(mCategoryName, _type);
		return object == nullptr ? nullptr : object->castType<ControllerItem>();
	}

	void ControllerManager::addItem(Widget* _widget, ControllerItem* _item)
	{
		// если виджет первый, то подписываемся на кадры
		if (mListItem.empty())
			Gui::getInstance().eventFrameStart += newDelegate(this, &ControllerManager::frameEntered);

		// подготавливаем
		_item->prepareItem(_widget);

		for (ListControllerItem::iterator iter = mListItem.begin(); iter != mListItem.end(); ++iter)
		{
			// такой уже в списке есть
			if ((*iter).first == _widget)
			{
				if ((*iter).second->getTypeName() == _item->getTypeName())
				{
					delete (*iter).second;
					(*iter).second = _item;
					return;
				}
			}
		}

		// вставляем в самый конец
		mListItem.push_back(PairControllerItem(_widget, _item));
	}

	void ControllerManager::removeItem(Widget* _widget)
	{
		// не удаляем из списка, а обнуляем, в цикле он будет удален
		for (ListControllerItem::iterator iter = mListItem.begin(); iter != mListItem.end(); ++iter)
		{
			if ((*iter).first == _widget) (*iter).first = nullptr;
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
				// удаляем из списка, итератор не увеличиваем и на новый круг
				iter = mListItem.erase(iter);
				continue;
			}

			if ((*iter).second->addTime((*iter).first, _time))
			{
				++iter;
				continue;
			}

			// на следующей итерации виджет вылетит из списка
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
