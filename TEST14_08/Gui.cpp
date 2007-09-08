
#include "Gui.h"
#include "LayoutManager.h"

namespace widget
{

	Gui::Gui() :
//		: BasisWidget(-100000, -100000, 100000, 100000, 0, 0),
		InputManager(/*m_widgetChild*/),
		m_widgetManagerInstance(WidgetManager::getInstance()),
		m_isInitialise(false),
//		m_skinManagerInstance(SkinManager::getInstance()),
//		m_layerManagerInstance(LayerManager::getInstance()),
//		m_layoutManagerInstance(LayoutManager::getInstance()),
//		m_pointerManagerInstance(PointerManager::getInstance()),
//		m_inputManagerInstance(InputManager::getInstance()),
		m_height(1), m_width(1)
	{
		// регистрируем фабрику текста и панели
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
//		m_factoryTextSimpleOverlay = new TextSimpleOverlayElementFactory();
		overlayManager.addOverlayElementFactory(&m_factoryTextSimpleOverlay);
//		m_factoryPanelAlphaOverlay = new PanelAlphaOverlayElementFactory();
		overlayManager.addOverlayElementFactory(&m_factoryPanelAlphaOverlay);

	}

	void Gui::initialise(Ogre::RenderWindow* _window)
	{
		Ogre::Viewport * port = _window->getViewport(0);
		m_height = port->getActualHeight();
		m_width = port->getActualWidth();
		m_isInitialise = true;
	}

	void Gui::shutdown()
	{
		if (!m_isInitialise) return;
		
		// очищаем уровни
		m_layerManagerInstance.clear();

		// удаляем выджеты
		destroyWidget();

		m_isInitialise = false;
	}

	WidgetPtr Gui::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = m_widgetManagerInstance.createWidget(_type, _skin, _x, _y, _cx, _cy, _align, 0, _name);
		m_widgetChild.push_back(widget);
		// присоединяем виджет с уровню
		m_layerManagerInstance.attachItem(widget, _layer, true);
		return widget;
	}

/*	void Gui::destroyWidget(WidgetPtr & _widget)
	{
		if (_widget == null) return;
		// отсоединяем виджет от уровня
		m_layerManagerInstance.detachItem(_widget);
		// и удаляем
		m_widgetManagerInstance.destroyWidget(_widget);
	}*/

/*	void Gui::destroyWidget()
	{
		for (WidgetChild::iterator iter = m_widgetChild.begin(); iter != m_widgetChild.end(); iter++) {
//			(*widget)->align(m_x, m_y, _cx, _cy, m_margin || margin);
//		while (m_widgetChild.size()) {
			WidgetPtr widget = iter;//m_widgetChild.back();
			m_layerManagerInstance.detachItem(widget);
			m_widgetManagerInstance.destroyWidget(widget);
//			m_widgetChild.pop_back();
		}
		m_widgetChild.clear();
	}*/

//	void Gui::attach(BasisWidgetPtr _basis/*, bool _child*/)
//	{
		// просто присоединяем элемент к оверлею
/*		OverlayManager &overlayManager = OverlayManager::getSingleton();
		Overlay * overlay;
		static long num=0;
		overlay = overlayManager.create(Ogre::StringConverter::toString(num++) + "_WidgetOverlay");
		overlay->setZOrder(0);
		overlay->show();
		overlay->add2D(static_cast<Ogre::OverlayContainer*>(_basis->getOverlayElement()));
		overlay->hide();*/
//		Overlay * overlay = m_layerManagerInstance.createOverlay();
//		overlay->add2D(static_cast<Ogre::OverlayContainer*>(_basis->getOverlayElement()));

		// подругому никак =(
//		LayerItemInfoPtr item = dynamic_cast<LayerItemInfoPtr>(_basis->getParent());
//		assert(item);
//		m_layerManagerInstance.attachItem(
//		item->setOverlay(overlay);

//	}
	// удяляет только негодных батюшке государю
	void Gui::destroyWidget(WidgetPtr & _widget)
	{
		for (size_t index = 0; index < m_widgetChild.size(); index++) {
			WidgetPtr widget = m_widgetChild[index];
			if (_widget == widget) {
				// удаляем свое имя
				WidgetManager::getInstance().clearName(_widget);
				delete _widget;
				_widget = 0;

				// удаляем из списка
				m_widgetChild[index] = m_widgetChild[m_widgetChild.size()-1];
				m_widgetChild.pop_back();
				return;
			}
		}
	}

	// удаляет всех детей
	void Gui::destroyWidget()
	{
		for (WidgetChild::iterator iter = m_widgetChild.begin(); iter != m_widgetChild.end(); iter++) {
			WidgetPtr widget = *iter;
			// удаляем свое имя
			WidgetManager::getInstance().clearName(widget);
			// отсоединяем виджет от уровня, если он был присоединен
			LayerManager::getInstance().detachItem(widget);
			// и удаляем
			delete widget;
		}
		m_widgetChild.clear();
	}

} // namespace widget