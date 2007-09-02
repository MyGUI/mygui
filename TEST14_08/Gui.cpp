
#include "Gui.h"
#include "LayoutManager.h"

namespace widget
{

	Gui::Gui()
		: BasisWidget(-100000, -100000, 100000, 100000, 0, 0),
		InputManager(/*m_widgetChild*/),
		m_widgetManagerInstance(WidgetManager::getInstance()),
		m_skinManagerInstance(SkinManager::getInstance()),
//		m_layerManagerInstance(LayerManager::getInstance()),
		m_layoutManagerInstance(LayoutManager::getInstance()),
//		m_pointerManagerInstance(PointerManager::getInstance()),
//		m_inputManagerInstance(InputManager::getInstance()),
		m_height(1), m_width(1)
	{
		// ������������ ������� ������ � ������
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
		m_factoryTextSimpleOverlay = new TextSimpleOverlayElementFactory();
		overlayManager.addOverlayElementFactory(m_factoryTextSimpleOverlay);
		m_factoryPanelAlphaOverlay = new PanelAlphaOverlayElementFactory();
		overlayManager.addOverlayElementFactory(m_factoryPanelAlphaOverlay);

		// ��������� ������ � �������� �������
		m_layerManagerInstance.load("main.layer");
		m_pointerManagerInstance.load("main.pointer");
	}

	void Gui::initialise(Ogre::RenderWindow* _window)
	{
		Ogre::Viewport * port = _window->getViewport(0);
		m_height = port->getActualHeight();
		m_width = port->getActualWidth();
	}

	void Gui::shutdown()
	{
		destroyWidget();
		// ������� ������
		m_layerManagerInstance.clear();
		// ������� �������
		delete m_factoryTextSimpleOverlay;
		delete m_factoryPanelAlphaOverlay;
	}

	WidgetPtr Gui::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = m_widgetManagerInstance.createWidget(_type, _skin, _x, _y, _cx, _cy, _align, this, _name);
		m_widgetChild.push_back(widget);
		// ������������ ������ � ������
		m_layerManagerInstance.attachItem(widget, _layer, true);
		return widget;
	}

	void Gui::destroyWidget(WidgetPtr & _widget)
	{
		if (_widget == null) return;
		// ����������� ������ �� ������
		m_layerManagerInstance.detachItem(_widget);
		// � �������
		m_widgetManagerInstance.destroyWidget(_widget);
	}

	void Gui::destroyWidget()
	{
		while (m_widgetChild.size()) {
			WidgetPtr wid = m_widgetChild.back();

			m_layerManagerInstance.detachItem(wid);
			m_widgetManagerInstance.destroyWidget(wid);

			m_widgetChild.pop_back();
		}
	}

	void Gui::attach(BasisWidgetPtr _basis, bool _child)
	{
		// ������ ������������ ������� � �������
/*		OverlayManager &overlayManager = OverlayManager::getSingleton();
		Overlay * overlay;
		static long num=0;
		overlay = overlayManager.create(Ogre::StringConverter::toString(num++) + "_WidgetOverlay");
		overlay->setZOrder(0);
		overlay->show();
		overlay->add2D(static_cast<Ogre::OverlayContainer*>(_basis->getOverlayElement()));
		overlay->hide();*/
		Overlay * overlay = m_layerManagerInstance.createOverlay();
		overlay->add2D(static_cast<Ogre::OverlayContainer*>(_basis->getOverlayElement()));

		// ��������� ����� =(
		LayerItemInfoPtr item = dynamic_cast<LayerItemInfoPtr>(_basis->getParent());
		assert(item);
		item->setOverlay(overlay);

	}


} // namespace widget