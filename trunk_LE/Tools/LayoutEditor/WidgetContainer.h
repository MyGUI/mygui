#ifndef __WIDGET_CONTAINER_H__
#define __WIDGET_CONTAINER_H__

#include "MyGUI.h"

namespace tools
{
	const std::string DEFAULT_EDITOR_LAYER = "LayoutEditor_Widgets";
	const std::string DEFAULT_LAYER = "Back";

	struct ControllerInfo
	{
		std::string mType;
		MyGUI::MapString mProperty;
	};

	struct WidgetContainer
	{
		WidgetContainer() :
			widget(nullptr),
			relative_mode(false)
		{ }
		WidgetContainer(const std::string& _type, const std::string& _skin, MyGUI::Widget* _widget, const std::string& _name = ""):
			widget(_widget),
			type(_type),
			skin(_skin),
			name(_name),
			layer(""),
			relative_mode(false)
		{ }

		~WidgetContainer()
		{
			for (std::vector<WidgetContainer*>::iterator iter = childContainers.begin(); iter != childContainers.end(); ++iter)
				delete *iter;
			childContainers.clear();

			for (std::vector<ControllerInfo*>::iterator iter = mController.begin(); iter != mController.end(); ++iter)
				delete *iter;
			mController.clear();
		}

		MyGUI::Widget* widget;
		std::vector<WidgetContainer*> childContainers;
		MyGUI::VectorStringPairs mProperty;
		MyGUI::VectorStringPairs mUserString;
		std::vector<ControllerInfo*> mController;
		std::string type;
		std::string skin;
		std::string align;
		std::string name;
		std::string layer;
		// not for saving
		std::string position(bool _percent = true)
		{
			if (relative_mode)
			{
				MyGUI::FloatCoord coord = MyGUI::CoordConverter::convertToRelative(widget->getCoord(), widget->getParentSize());
				std::ostringstream stream;
				if (_percent) stream << coord.left * 100 << " " << coord.top * 100 << " " << coord.width * 100 << " " << coord.height * 100;
				else stream << coord.left << " " << coord.top << " " << coord.width << " " << coord.height;
				return stream.str();
			}
			return widget->getCoord().print();
		}
		bool relative_mode;
	};

} // namespace tools

#endif // __WIDGET_CONTAINER_H__
