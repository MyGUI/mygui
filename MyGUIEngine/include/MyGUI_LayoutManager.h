/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_LAYOUT_MANAGER_H_
#define MYGUI_LAYOUT_MANAGER_H_

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
		EventHandle_AddUserStringDelegate eventAddUserString;

		const std::string& getCurrentLayout() const;

	private:
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

	private:
		bool mIsInitialise;
		std::string mCurrentLayoutName;
		std::string mXmlLayoutTagName;
	};

} // namespace MyGUI

#endif // MYGUI_LAYOUT_MANAGER_H_
