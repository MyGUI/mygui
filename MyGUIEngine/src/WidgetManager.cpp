
#include "WidgetManager.h"
#include "LayerManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(WidgetManager);
/*	template<> WidgetManager* Instance<WidgetManager>::msInstance = 0;
	WidgetManager* WidgetManager::getInstancePtr(void) {return msInstance;}
	WidgetManager& WidgetManager::getInstance(void)
	{
		assert(msInstance);
		return (*msInstance);
	}*/

	WidgetManager::WidgetManager() {};

	WidgetPtr WidgetManager::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, BasisWidgetPtr _parent, const Ogre::String & _name)
	{
		Ogre::String name;
		if (!_name.empty()) {
			mapWidgetPtr::iterator iter = m_widgets.find(_name);
			if (iter != m_widgets.end()) OGRE_EXCEPT(0, _name + " - name widget is exist", "WidgetManager::createWidget");
			name = _name;
		} else {
			static long num=0;
			name = Ogre::StringConverter::toString(num++) + "_" + _type;
		}

		for (std::list<WidgetFactoryBase*>::iterator factory = m_factoryList.begin(); factory != m_factoryList.end(); factory++) {
			if ( (*factory)->getType() == _type) {
				WidgetPtr widget = (*factory)->createWidget(_skin, _x, _y, _cx, _cy, _align, _parent, name);
				m_widgets[name] = widget;
				return widget;
			}
		}
		OGRE_EXCEPT(0, _type + " - no find factory WidgetFactory", "WidgetManager::createWidget");
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

	floatRect WidgetManager::convertOffset(const floatRect & _offset, char _align, const intSize & _parentSkinSize, int _parentWidth, int _parentHeight)
	{
		floatRect offset = _offset;

		if (_align & ALIGN_RIGHT) {
			if (_align & ALIGN_LEFT) offset.right += _parentWidth - _parentSkinSize.width;
			else offset.left += _parentWidth - _parentSkinSize.width;
		} else if (!(_align & ALIGN_LEFT)) offset.left += (_parentWidth - _parentSkinSize.width) / 2;

		if (_align & ALIGN_BOTTOM) {
			if (_align & ALIGN_TOP) offset.bottom += _parentHeight - _parentSkinSize.height;
			else offset.top += _parentHeight - _parentSkinSize.height;
		} else if (!(_align & ALIGN_TOP)) offset.top += (_parentHeight - _parentSkinSize.height) / 2;

		return offset;
	}

} // namespace MyGUI