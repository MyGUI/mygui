
#include "WidgetManager.h"
#include "LayerManager.h"

namespace MyGUI
{

	WidgetPtr WidgetManager::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, BasisWidgetPtr _parent, const Ogre::String & _name)
	{
		Ogre::String name;
		if (!_name.empty()) {
			mapWidgetPtr::iterator iter = m_widgets.find(_name);
			if (iter != m_widgets.end()) assert(0 && "name widget is exist");
			name = _name;
		} else {
			static long num=0;
			name = Ogre::StringConverter::toString(num++) + "_" + _type;
		}

		for (std::list<WidgetFactoryBase*>::iterator factory = m_factoryList.begin(); factory != m_factoryList.end(); factory++) {
			if ((*factory)->getType() == _type) {
				WidgetPtr widget = (*factory)->createWidget(_skin, _x, _y, _cx, _cy, _align, _parent, name);
				m_widgets[name] = widget;
				return widget;
			}
		}
		assert(0 && "no find factory WidgetFactory");
		return 0;
	}

	void WidgetManager::destroyWidget(WidgetPtr & _widget)
	{
		if (_widget == null) return;
		// отсоединяем виджет от уровня, если он был присоединен
		LayerManager::getInstance().detachItem(_widget);

		// стираем имя в карте для поиска
		clearName(_widget);

		// если отца нет то отец гуй
		WidgetPtr parent = _widget->getParent();
		if (parent == null) Gui::getInstance().destroyWidget(_widget);
		else parent->destroyWidget(_widget);

		_widget = null;
	}

	void WidgetManager::destroyWidget()
	{
		Gui::getInstance().destroyWidget();
	}

	WidgetPtr WidgetManager::findWidget(const Ogre::String & _name)
	{
		mapWidgetPtr::iterator iter = m_widgets.find(_name);
		if (iter == m_widgets.end()) return 0;
		return iter->second;
	}

	void WidgetManager::clearName(WidgetPtr _widget)
	{
		if (_widget == null) return;
		mapWidgetPtr::iterator iter = m_widgets.find(_widget->getName());
		if (iter != m_widgets.end()) m_widgets.erase(iter);
	}

} // namespace MyGUI