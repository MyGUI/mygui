
#include "Gui.h"

namespace widget
{

	Gui::Gui()
		: BasisWidget(-100000, -100000, 100000, 100000, 0, 0)
	{
	}

	WidgetPtr Gui::createChild(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _x, _y, _cx, _cy, _align, this, _name);
		m_widgetChild.push_back(widget);

		return widget;
	}

	void Gui::attach(OverlayElementPtr _element, bool _child)
	{
		// а если нет, то создадим отца
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		Overlay * overlay;
		overlay = overlayManager.create("WidgetOverlay");
		overlay->setZOrder(200);
		overlay->show();
		overlay->add2D(static_cast<Ogre::OverlayContainer*>(_element));
	}

} // namespace widget