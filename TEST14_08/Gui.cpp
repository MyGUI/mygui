
#include "Gui.h"
#include "LayerManager.h"

namespace widget
{

	Gui::Gui()
		: BasisWidget(-100000, -100000, 100000, 100000, 0, 0)
	{
		// загружаем уровни в менеджер уровней
		LayerManager::getSingleton().load("main.layer");

	}

	WidgetPtr Gui::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _x, _y, _cx, _cy, _align, this, _name);
		m_widgetChild.push_back(widget);

		// присоединяем виджет с уровню
		LayerManager::getSingleton().attachWidget(widget, _layer);

		return widget;
	}

	void Gui::destroyWidget(WidgetPtr & _widget)
	{
		// отсоединяем виджет от уровня
		LayerManager::getSingleton().detachWidget(_widget);
		// и удаляем
		WidgetManager::getInstance().destroyWidget(_widget);
	}

	void Gui::attach(BasisWidgetPtr _basis, bool _child)
	{
		// просто присоединяем элемент к оверлею
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		Overlay * overlay;
		static long num=0;
		overlay = overlayManager.create(Ogre::StringConverter::toString(num++) + "_WidgetOverlay");
		overlay->setZOrder(0);
		overlay->show();
		overlay->add2D(static_cast<Ogre::OverlayContainer*>(_basis->getOverlayElement()));
		overlay->hide();

		// подругому никак =(
		LayerItemInfoPtr item = dynamic_cast<LayerItemInfoPtr>(_basis->getParent());
		assert(item);
		item->m_overlayInfo = overlay;

//		_element->_setParent((Ogre::OverlayContainer*)overlay);

		// ищем кому принадлежит этот оверлей, так как элемент не возвращает оверлей
/*		for (WidgetChild::iterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) {
			if ((*widget)->getOverlayElement() == _element) {
				(*widget)->m_overlayInfo = overlay;
				break;
			}
		}*/

	}

} // namespace widget