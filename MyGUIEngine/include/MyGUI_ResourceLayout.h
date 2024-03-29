/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_RESOURCE_LAYOUT_H_
#define MYGUI_RESOURCE_LAYOUT_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IResource.h"
#include "MyGUI_LayoutData.h"

namespace MyGUI
{

	/** ResourceLayout is resource that contain full layout info, used for creating widgets. */
	class MYGUI_EXPORT ResourceLayout : public IResource
	{
		MYGUI_RTTI_DERIVED(ResourceLayout)

	public:
		ResourceLayout() = default;
		ResourceLayout(xml::ElementPtr _node, std::string_view _file);

		void deserialization(xml::ElementPtr _node, Version _version) override;

		const VectorWidgetInfo& getLayoutData() const;

		/** Create widgets described in layout */
		VectorWidgetPtr createLayout(std::string_view _prefix = {}, Widget* _parent = nullptr);

		// widget info -> Widget
		Widget* createWidget(
			const WidgetInfo& _widgetInfo,
			std::string_view _prefix = {},
			Widget* _parent = nullptr,
			bool _template = false);

	protected:
		// xml -> widget info
		WidgetInfo parseWidget(xml::ElementEnumerator& _widget);

	protected:
		VectorWidgetInfo mLayoutData;
	};

} // namespace MyGUI

#endif // MYGUI_RESOURCE_LAYOUT_H_
