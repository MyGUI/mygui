#pragma once

#include <Ogre.h>
#include <string>
#include "xmlDocument.h"
#include "Widget.h"
#include "Instance.h"


namespace widget
{

	class LayoutManager
	{
	private:
		LayoutManager();
		~LayoutManager();

	public:
		INSTANCE(LayoutManager)

		void load(const std::string & _file);

	private:
		void parseLayoutMyGUI(xml::xmlNodePtr _root);
		void parseLayoutCEGUI(xml::xmlNodePtr _root);

		void parseWidgetMyGUI(xml::xmlNodePtr _widgetInfo, WidgetPtr _parent);
		
	}; // class LayoutManager

} // namespace widget