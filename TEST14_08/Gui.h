#pragma once

#include <vector>
#include <Ogre.h>
#include "Instance.h"
#include "InputManager.h"
#include "WidgetManager.h"
//#include "SkinManager.h"
//#include "LayerManager.h"
//#include "LayoutManager.h"

#include "TextSimpleOverlayElementFactory.h"
#include "PanelAlphaOverlayElementFactory.h"

namespace widget
{
	class InputManager;
	class WidgetManager;

	class Gui : /*public BasisWidget, */public InputManager//, public Ogre::WindowEventListener
	{

		friend WidgetManager;

	private:
		Gui();
//		~Gui();

	public:
		INSTANCE(Gui)

		void initialise(Ogre::RenderWindow* _window);
		void shutdown();

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _x, float _y, float _cx, float _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidget(_type, _skin, (int)(_x*m_width), (int)(_y*m_height), (int)(_cx*m_width), (int)(_cy*m_height), _align, _layer, _name);
		}

//		void destroyWidget(WidgetPtr & _widget);
//		void destroyWidget();

//		void attach(BasisWidgetPtr _basis/*, bool _child*/);

		// копии методов у менеджеров
/*		inline WidgetPtr findWidget(const std::string & _name)
		{
			return m_widgetManagerInstance.findWidget(_name);
		}
		inline void loadSkin(const std::string & _file)
		{
			m_skinManagerInstance.loadSkin(_file);
		}
		inline void loadLayout(const std::string & _file)
		{
			m_layoutManagerInstance.loadLayout(_file);
		}*/

		inline floatRect convertToReal(const floatRect & _rect)
		{
			return floatRect(_rect.left*m_width, _rect.top*m_height, _rect.right*m_width, _rect.bottom*m_height);
		}

/*		inline bool injectMouseMove( const OIS::MouseEvent &arg )
		{
			return injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.rel);
		}
		inline bool injectMousePress( const OIS::MouseEvent &arg , OIS::MouseButtonID id )
		{
			return injectMousePress(arg.state.X.abs, arg.state.Y.abs, id == OIS::MB_Left);
		}
		inline bool injectMouseRelease( const OIS::MouseEvent &arg , OIS::MouseButtonID id )
		{
			return injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, id == OIS::MB_Left);
		}*/

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

//		SkinManager & m_skinManagerInstance;
		WidgetManager & m_widgetManagerInstance;
//		LayerManager & m_layerManagerInstance;
//		LayoutManager & m_layoutManagerInstance;
//		PointerManager & m_pointerManagerInstance;
//		InputManager & m_inputManagerInstance;

		// фабрики наших оверлеев
		TextSimpleOverlayElementFactory m_factoryTextSimpleOverlay;
		PanelAlphaOverlayElementFactory m_factoryPanelAlphaOverlay;

		float m_height, m_width;
		bool m_isInitialise;

	}; // class Gui

} // namespace widget