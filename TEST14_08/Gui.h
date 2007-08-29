#pragma once

#include <vector>
#include <Ogre.h>
#include "WidgetManager.h"
#include "SkinManager.h"
#include "LayerManager.h"
#include "LayoutManager.h"
#include "Instance.h"

namespace widget
{

	class Gui : public BasisWidget
	{

	private:
		Gui();

	public:
		INSTANCE(Gui)

		void initialise(Ogre::RenderWindow* _window);

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _x, float _y, float _cx, float _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidget(_type, _skin, (int)(_x*m_width), (int)(_y*m_height), (int)(_cx*m_width), (int)(_cy*m_height), _align, _layer, _name);
		}

		void destroyWidget(WidgetPtr & _widget);

		void attach(BasisWidgetPtr _basis, bool _child);

		// копии методов у менеджеров
		inline WidgetPtr findWidget(const std::string & _name)
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
		}

		inline floatRect convertToReal(const floatRect & _rect)
		{
			return floatRect(_rect.left*m_width, _rect.top*m_height, _rect.right*m_width, _rect.bottom*m_height);
		}

		inline float getWidth() {return m_width;}
		inline float getHeight() {return m_height;}

	private:
		// вектор всех детей виджетов
		WidgetChild m_widgetChild;

		SkinManager & m_skinManagerInstance;
		WidgetManager & m_widgetManagerInstance;
		LayerManager & m_layerManagerInstance;
		LayoutManager & m_layoutManagerInstance;

		float m_height, m_width;

	}; // class Gui

} // namespace widget