/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_RESOURCE_LAYOUT_H__
#define __MYGUI_RESOURCE_LAYOUT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IResource.h"
#include "MyGUI_LayoutData.h"

namespace MyGUI
{

	/** ResourceLayout is resource that contain full layout info, used for creating widgets. */
	class MYGUI_EXPORT ResourceLayout :
		public IResource
	{
		MYGUI_RTTI_DERIVED( ResourceLayout )

	public:
		ResourceLayout();

		ResourceLayout(xml::ElementPtr _node, const std::string& _file);

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		const VectorWidgetInfo& getLayoutData() const;

		/** Create widgets described in layout */
		VectorWidgetPtr createLayout(const std::string& _prefix = "", Widget* _parent = nullptr);

		// widget info -> Widget
		Widget* createWidget(const WidgetInfo& _widgetInfo, const std::string& _prefix = "", Widget* _parent = nullptr, bool _template = false);

	protected:
		// xml -> widget info
		WidgetInfo parseWidget(xml::ElementEnumerator& _widget);

	protected:
		VectorWidgetInfo mLayoutData;
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_LAYOUT_H__
