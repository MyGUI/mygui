#ifndef _LAYOUTMANAGER_H_
#define _LAYOUTMANAGER_H_

#include "Prerequest.h"
#include <Ogre.h>
#include <string>
#include "Common.h"
#include "xmlDocument.h"
#include "WidgetDefines.h"
#include "Instance.h"


namespace MyGUI
{

	class _MyGUIExport LayoutManager
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

} // namespace MyGUI

#endif // _LAYOUTMANAGER_H_