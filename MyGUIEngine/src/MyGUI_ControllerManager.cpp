/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(ControllerManager);

	void ControllerManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		WidgetManager::getInstance().registerUnlinker(this);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void ControllerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		WidgetManager::getInstance().unregisterUnlinker(this);
		clear();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void ControllerManager::clear()
	{
		for (ListControllerItem::iterator iter=mListItem.begin(); iter!=mListItem.end(); ++iter) {
			delete (*iter).second;
		}
		mListItem.clear();
	}

	void ControllerManager::addItem(WidgetPtr _widget, ControllerItem * _item)
	{
		// если виджет первый, то подписываемся на кадры
		if (0 == mListItem.size()) Gui::getInstance().addFrameListener(newDelegate(this, &ControllerManager::frameEntered));

		// подготавливаем
		_item->prepareItem(_widget);

		for (ListControllerItem::iterator iter=mListItem.begin(); iter!=mListItem.end(); ++iter) {

			// такой уже в списке есть
			if ((*iter).first == _widget) {
				if ((*iter).second->getType() == _item->getType()) {
					(*iter).second->replaseItem((*iter).first, _item);
					delete _item;
					return;
				}
			}
		}

		// вставляем в самый конец
		mListItem.push_back(PairControllerItem(_widget, _item));
	}

	void ControllerManager::removeItem(WidgetPtr _widget)
	{
		// не удаляем из списка, а обнуляем, в цикле он будет удален
		for (ListControllerItem::iterator iter=mListItem.begin(); iter!=mListItem.end(); ++iter) {
			if ((*iter).first == _widget) (*iter).first = null;
		}
	}

	void ControllerManager::_unlinkWidget(WidgetPtr _widget)
	{
		removeItem(_widget);
	}

	void ControllerManager::frameEntered(float _time)
	{
		for (ListControllerItem::iterator iter=mListItem.begin(); iter!=mListItem.end(); /*added in body*/) {

			if (null == (*iter).first) {
				delete (*iter).second;
				// удаляем из списка, итератор не увеличиваем и на новый круг
				iter = mListItem.erase(iter);
				continue;
			}

			if ((*iter).second->addTime((*iter).first, _time)) {
				++iter;
				continue;
			}

			// на следующей итерации виджет вылетит из списка
			(*iter).first = null;
		}

		if (0 == mListItem.size()) Gui::getInstance().removeFrameListener(newDelegate(this, &ControllerManager::frameEntered));
	}

} // namespace MyGUI
