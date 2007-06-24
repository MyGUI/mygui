#include "Widget.h"

namespace widget
{

	Widget::Widget(int _x, int _y, int _cx, int _cy, Widget * _parent)
		: m_parent (_parent),
		m_x (_x),
		m_y (_y),
		m_cx (_cx),
		m_cy (_cy),
		m_view_x (_x),
		m_view_y (_y),
		m_view_cx (_cx),
		m_view_cy (_cy)
	{
		check();
	}

	Widget::~Widget()
	{
		while (m_widgetChild.size() > 0)  {
			delete m_widgetChild[0];
			m_widgetChild.pop_back();
		};
	}

	Widget * Widget::createChild(int _x, int _y, int _cx, int _cy)
	{
		Widget * widget = new Widget(_x, _y, _cx, _cy, this);
		m_widgetChild.push_back(widget);
		return widget;
	}

	void Widget::size(int _cx, int _cy)
	{
		m_cx = _cx;
		m_cy = _cy;
		check();
	}

	void Widget::move(int _x, int _y)
	{
		m_x = _x;
		m_y = _y;

		m_overlayContainer->setPosition(m_x, m_y);
		check();
	}

	void Widget::check()
	{
		if (!m_parent) return;

	}

	void Widget::createOverlay(Ogre::Overlay * _overlay, Ogre::OverlayContainer * _overlayContainer, const Ogre::String & material)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement(
			"Panel", "Widget_" + material) );

		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_x, m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setMaterialName(material);

		if (_overlayContainer) _overlayContainer->addChild(m_overlayContainer);
		else _overlay->add2D(m_overlayContainer);

	}


} // namespace Widget