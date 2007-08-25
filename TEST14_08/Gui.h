#pragma once

#include <vector>
#include <Ogre.h>
#include "WidgetManager.h"

namespace widget
{

	class Gui : public BasisWidget
	{

	private:
		Gui();

	public:
		inline static Gui & getInstance()
		{
			static Gui instance;
			return instance;
		}

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name = "");
		void destroyWidget(WidgetPtr & _widget);

		void attach(BasisWidgetPtr _basis, bool _child);

	private:
		// вектор всех детей виджетов
		WidgetChild m_widgetChild;

	}; // class Gui

} // namespace widget