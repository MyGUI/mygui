/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LAYOUT_MANAGER_H__
#define __MYGUI_LAYOUT_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Common.h"
#include "xmlDocument.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	class _MyGUIExport LayoutManager
	{
		INSTANCE_HEADER(LayoutManager);

	public:
		void initialise();
		void shutdown();

		VectorWidgetPtr load(const std::string & _file, bool _resource = true);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

	private:
		void parseLayout(VectorWidgetPtr & _widgets, xml::xmlNodePtr _root);
		void parseWidget(VectorWidgetPtr & _widgets, xml::xmlNodeIterator & _widget, WidgetPtr _parent);
		FloatCoord convertFromReal(const FloatCoord & _coord, WidgetPtr _parent);

		// для возврата последней загрузки
		VectorWidgetPtr mVectorWidgetPtr;
		
	}; // class LayoutManager

} // namespace MyGUI

#endif // __MYGUI_LAYOUT_MANAGER_H__
