/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_LAYOUT_MANAGER_H_
#define MYGUI_LAYOUT_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ResourceLayout.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	using EventHandle_CreateWidgetDelegate = delegates::MultiDelegate<Widget*, const WidgetInfo&>;
	using EventHandle_AddUserStringDelegate = EventPairConvertStringView<
		delegates::MultiDelegate<Widget*, const std::string&, const std::string&>,
		delegates::MultiDelegate<Widget*, std::string_view, std::string_view>>;

	class MYGUI_EXPORT LayoutManager : public MemberObsolete<LayoutManager>
	{
		MYGUI_SINGLETON_DECLARATION(LayoutManager);

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
		VectorWidgetPtr loadLayout(std::string_view _file, std::string_view _prefix = {}, Widget* _parent = nullptr);

		/** Unload layout (actually deletes vector of widgets returned by loadLayout) */
		void unloadLayout(VectorWidgetPtr& _widgets);

		/** Get ResourceLayout by name */
		ResourceLayout* getByName(std::string_view _name, bool _throw = true) const;

		/** Check if skin with specified name exist */
		bool isExist(std::string_view _name) const;

		/** Event : Multidelegate. Widget was created from layout.\n
		signature : void method(MyGUI::Widget* _sender, const MyGUI::WidgetInfo& _widgetInfo)
		@param _widget Widget that got new UserString.
		@param _key UserString key.
		@param _key UserString value.
		@note Happens only when Widget was loaded from layout, but not when it was created in code.
		*/
		EventHandle_CreateWidgetDelegate eventCreateWidget;

		/** Event : Multidelegate. UserString was added from layout.\n
			signature : void method(MyGUI::Widget* _widget, std::string_view _key, std::string_view _value)
			@param _widget Widget that got new UserString.
			@param _key UserString key.
			@param _key UserString value.
			@note Happens only when UserString was loaded from layout, but not when it was added in code.
		*/
		EventHandle_AddUserStringDelegate eventAddUserString;

		const std::string& getCurrentLayout() const;

	private:
		void _load(xml::ElementPtr _node, std::string_view _file, Version _version);

	private:
		bool mIsInitialise{false};
		std::string mCurrentLayoutName;
		std::string mXmlLayoutTagName;
	};

} // namespace MyGUI

#endif // MYGUI_LAYOUT_MANAGER_H_
