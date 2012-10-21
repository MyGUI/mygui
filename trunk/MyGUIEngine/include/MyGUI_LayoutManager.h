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
#include "MyGUI_ResourceLayout.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	typedef delegates::CMultiDelegate3<Widget*, const std::string&, const std::string&> EventHandle_AddUserStringDelegate;

	class MYGUI_EXPORT LayoutManager :
		public Singleton<LayoutManager>,
		public MemberObsolete<LayoutManager>
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
		VectorWidgetPtr loadLayout(const std::string& _file, const std::string& _prefix = "", Widget* _parent = nullptr);

		/** Unload layout (actually deletes vector of widgets returned by loadLayout) */
		void unloadLayout(VectorWidgetPtr& _widgets);

		/** Get ResourceLayout by name */
		ResourceLayout* getByName(const std::string& _name, bool _throw = true) const;

		/** Check if skin with specified name exist */
		bool isExist(const std::string& _name) const;

		/** Event : Multidelegate. UserString was added from layout.\n
			signature : void method(MyGUI::Widget* _widget, const std::string& _key, const std::string& _value)
			@param _widget Widget that got new UserString.
			@param _key UserString key.
			@param _key UserString value.
			@note Happens only when UserString was loaded from layout, but not when it was added in code.
		*/
		EventHandle_AddUserStringDelegate
			eventAddUserString;

		const std::string& getCurrentLayout() const;

	private:
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

	private:
		bool mIsInitialise;
		std::string mCurrentLayoutName;
		std::string mXmlLayoutTagName;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYOUT_MANAGER_H__
