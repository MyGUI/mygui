/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_WIDGET_CONTAINER_H_
#define MYGUI_WIDGET_CONTAINER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_WidgetStyle.h"
#include "MyGUI_Align.h"
#include "MyGUI_TCoord.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class Widget;

	class MYGUI_EXPORT WidgetContainer
	{
	public:
		WidgetContainer() = default;
		virtual ~WidgetContainer() = default;

		/*internal:*/
		virtual void _linkChildWidget(Widget* _widget);
		void _unlinkChildWidget(Widget* _widget);

		// child widgets access
		const VectorWidgetPtr& getChildWidgets() const;
		size_t getChildCount() const;
		Widget* getChildAt(size_t _index) const;

	protected:
		virtual void _destroyChildWidget(Widget* _widget);
		void _destroyAllChildWidget();

		// widget creation (overridable for subclasses with different creation logic)
		virtual Widget* baseCreateWidget(
			WidgetStyle _style,
			std::string_view _type,
			std::string_view _skin,
			const IntCoord& _coord,
			Align _align,
			std::string_view _layer,
			std::string_view _name);

		VectorWidgetPtr mWidgetChild;
	};

} // namespace MyGUI

#endif // MYGUI_WIDGET_CONTAINER_H_
