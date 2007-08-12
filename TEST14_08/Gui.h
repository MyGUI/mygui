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

		WidgetPtr createChild(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _name = "");

		void attach(OverlayElementPtr _element, bool _child);

		inline void destroyWidget(WidgetPtr & _widget)
		{
			WidgetManager::getInstance().destroyWidget(_widget);
		}

	private:
		// ������ ���� ����� ��������
		WidgetChild m_widgetChild;

	}; // class Gui

} // namespace widget