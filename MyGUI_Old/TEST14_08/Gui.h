#pragma once

#include "Common.h"
#include "Instance.h"
#include "InputManager.h"

#include "TextSimpleOverlayElementFactory.h"
#include "PanelAlphaOverlayElementFactory.h"

namespace widget
{
	class WidgetManager;

	class Gui : public InputManager
	{

		friend WidgetManager;

	private:
		Gui();

	public:
		INSTANCE(Gui)

		void initialise(Ogre::RenderWindow* _window);
		void shutdown();

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _x, float _y, float _cx, float _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidget(_type, _skin, (int)(_x*m_width), (int)(_y*m_height), (int)(_cx*m_width), (int)(_cy*m_height), _align, _layer, _name);
		}

		inline floatRect convertToReal(const floatRect & _rect)
		{
			return floatRect(_rect.left*m_width, _rect.top*m_height, _rect.right*m_width, _rect.bottom*m_height);
		}

		inline float getWidth() {return m_width;}
		inline float getHeight() {return m_height;}

//	private:
		// удяляет только негодных батюшке государю
		void destroyWidget(WidgetPtr & _widget);
		// удаляет всех детей
		void destroyWidget();

	private:
		// вектор всех детей виджетов
		WidgetChild m_widgetChild;

		// фабрики наших оверлеев
		TextSimpleOverlayElementFactory m_factoryTextSimpleOverlay;
		PanelAlphaOverlayElementFactory m_factoryPanelAlphaOverlay;

		float m_height, m_width;
		bool m_isInitialise;

	}; // class Gui

} // namespace widget