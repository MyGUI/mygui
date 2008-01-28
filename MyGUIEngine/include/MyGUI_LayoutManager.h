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
#include "MyGUI_XmlDocument.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	class _MyGUIExport LayoutManager
	{
		INSTANCE_HEADER(LayoutManager);

	public:
		void initialise();
		void shutdown();

		VectorWidgetPtr load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

		/* Convert from relative to pixel coordinates.
			@param
				_coord relative coordinates. (relative to _parent client area coordinates)
			@param
				_parent Widget.
		*/
		IntCoord convertRelativeToInt(const FloatCoord & _coord, WidgetPtr _parent);
		/* Convert from pixel to relative coordinates.
			@param
				_coord relative coordinates. (relative to _parent client area coordinates)
			@param
				_parent Widget.
		*/
		FloatCoord convertIntToRelative(const IntCoord & _coord, WidgetPtr _parent);
	private:
		void parseLayout(VectorWidgetPtr & _widgets, xml::xmlNodePtr _root);
		void parseWidget(VectorWidgetPtr & _widgets, xml::xmlNodeIterator & _widget, WidgetPtr _parent);

		// для возврата последней загрузки
		VectorWidgetPtr mVectorWidgetPtr;
		
	}; // class LayoutManager

} // namespace MyGUI

#endif // __MYGUI_LAYOUT_MANAGER_H__
