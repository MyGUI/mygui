/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldPosition.h"
#include "Parse.h"
#include "Localise.h"
#include "EditorWidgets.h"

namespace tools
{

	PropertyFieldPosition::PropertyFieldPosition(MyGUI::Widget* _parent) :
		PropertyFieldEditBox(_parent)
	{
	}

	PropertyFieldPosition::~PropertyFieldPosition()
	{
	}

	void PropertyFieldPosition::onAction(const std::string& _value, bool _force)
	{
		const std::string DEFAULT_STRING = "[DEFAULT]";
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		EditorWidgets* ew = &EditorWidgets::getInstance();
		WidgetContainer* widgetContainer = ew->find(mCurrentWidget);

		bool goodData = onCheckValue();

		if (goodData)
		{
			if (widgetContainer->relative_mode)
			{
				std::istringstream str(_value);
				MyGUI::FloatCoord float_coord;
				str >> float_coord;
				float_coord.left = float_coord.left / 100;
				float_coord.top = float_coord.top / 100;
				float_coord.width = float_coord.width / 100;
				float_coord.height = float_coord.height / 100;
				MyGUI::IntCoord coord = MyGUI::CoordConverter::convertFromRelative(float_coord, mCurrentWidget->getParentSize());
				mCurrentWidget->setCoord(coord);

				EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");
			}
			else
			{
				widgetContainer->widget->setProperty("Coord", _value);
				EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");
			}
		}
	}

	bool PropertyFieldPosition::onCheckValue()
	{
		bool success = true;

		if (EditorWidgets::getInstance().find(mCurrentWidget)->relative_mode)
			success = utility::checkParse<float>(mField, 4);
		else
			success = utility::checkParse<int>(mField, 4);

		return success;
	}

} // namespace tools
