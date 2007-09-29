
#include "Gui.h"
#include "LayoutManager.h"
#include "LayerManager.h"
#include "WidgetManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(Gui);

	Gui::Gui() :
		m_isInitialise(false),
		m_height(1), m_width(1)
	{
		// ������������ ������� ������ � ������
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
		overlayManager.addOverlayElementFactory(&m_factoryTextSimpleOverlay);
		overlayManager.addOverlayElementFactory(&m_factoryPanelAlphaOverlay);
		overlayManager.addOverlayElementFactory(&mFactoryPanelAlphaSharedOverlay);
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
		
		// ������� ������
		LayerManager::getInstance().clear();

		// ������� �������
		destroyWidget();

		m_isInitialise = false;
	}

	WidgetPtr Gui::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _x, _y, _cx, _cy, _align, 0, _name);
		m_widgetChild.push_back(widget);
		// ������������ ������ � ������
		LayerManager::getInstance().attachItem(widget, _layer, true);
		return widget;
	}

	// ������� ������ �������� ������� ��������
	void Gui::destroyWidget(WidgetPtr & _widget)
	{
		for (size_t index = 0; index < m_widgetChild.size(); index++) {
			WidgetPtr widget = m_widgetChild[index];
			if (_widget == widget) {
				// ������� ���� ���
				WidgetManager::getInstance().clearName(_widget);
				delete _widget;
				_widget = 0;

				// ������� �� ������
				m_widgetChild[index] = m_widgetChild[m_widgetChild.size()-1];
				m_widgetChild.pop_back();
				return;
			}
		}
	}

	// ������� ���� �����
	void Gui::destroyWidget()
	{
		for (WidgetChild::iterator iter = m_widgetChild.begin(); iter != m_widgetChild.end(); iter++) {
			WidgetPtr widget = *iter;
			// ������� ���� ���
			WidgetManager::getInstance().clearName(widget);
			// ����������� ������ �� ������, ���� �� ��� �����������
			LayerManager::getInstance().detachItem(widget);
			// � �������
			delete widget;
		}
		m_widgetChild.clear();
	}

} // namespace MyGUI