/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_LAYOUT_MANAGER_H__
#define __MYGUI_LAYOUT_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	class MYGUI_EXPORT LayoutManager :
		public Singleton<LayoutManager>
	{
	public:
		LayoutManager();

		void initialise();
		void shutdown();

		/** Load layout file
			@param _file name of layout
			@param _prefix will be added to all loaded widgets names
			@param _parent widget to load on
			@return Return vector of pointers of loaded root widgets (root == without parents)
		*/
		VectorWidgetPtr& loadLayout(const std::string& _file, const std::string& _prefix = "", Widget* _parent = nullptr);

		/** Unload layout (actually deletes vector of widgets returned by loadLayout) */
		void unloadLayout(VectorWidgetPtr& _widgets);

	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : VectorWidgetPtr& LayoutManager::loadLayout(const std::string& _file, const std::string& _prefix, Widget* _parent)")
		VectorWidgetPtr& load(const std::string& _file) { return loadLayout(_file); }

#endif // MYGUI_DONT_USE_OBSOLETE

	private:
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

		void parseLayout(VectorWidgetPtr& _widgets, xml::ElementPtr _root);
		void parseWidget(VectorWidgetPtr& _widgets, xml::ElementEnumerator& _widget, Widget* _parent);

	private:
		// для возврата последней загрузки
		VectorWidgetPtr mVectorWidgetPtr;

		// префикс при загрузке лейаута
		std::string layoutPrefix;
		// префикс при загрузке лейаута
		Widget* layoutParent;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYOUT_MANAGER_H__
