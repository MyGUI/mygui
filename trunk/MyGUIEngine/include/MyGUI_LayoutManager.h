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
#include "MyGUI_Gui.h"

namespace MyGUI
{

	//std::

	class _MyGUIExport LayoutManager
	{
		MYGUI_INSTANCE_HEADER(LayoutManager);

	public:
		void initialise();
		void shutdown();

		/** Load layout file
			@param _file name of layout
			@param _group Ogre resource group where _file is
			@return Return vector of pointers of loaded root widgets (root == without parents)
		*/
		VectorWidgetPtr load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file, Version _version);

		/** Load layout file
			@param _file name of layout
			@param _prefix will be added to all loaded widgets names
			@param _parent widget to load on
			@param _group Ogre resource group where _file is
			@return Return vector of pointers of loaded root widgets (root == without parents)
		*/
		VectorWidgetPtr loadLayout(const std::string & _file, const std::string & _prefix = "", WidgetPtr _parent = NULL, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		/** Unload layout file */
		void unloadLayout(VectorWidgetPtr & _widgets);

	private:
		void parseLayout(VectorWidgetPtr & _widgets, xml::xmlNodePtr _root);
		void parseWidget(VectorWidgetPtr & _widgets, xml::xmlNodeIterator & _widget, WidgetPtr _parent);

	private:
		// для возврата последней загрузки
		VectorWidgetPtr mVectorWidgetPtr;

		// префикс при загрузке лейаута
		std::string layoutPrefix;
		// префикс при загрузке лейаута
		WidgetPtr layoutParent;
		
	}; // class LayoutManager

} // namespace MyGUI

#endif // __MYGUI_LAYOUT_MANAGER_H__
