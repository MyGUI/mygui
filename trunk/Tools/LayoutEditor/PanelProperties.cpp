/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "precompiled.h"
#include "Localise.h"
#include "PanelProperties.h"
#include "EditorWidgets.h"

namespace tools
{
	PanelProperties::PanelProperties() : BasePanelViewItem("PanelProperties.layout")
	{
	}

	void PanelProperties::initialise()
	{
		mPanelCell->setCaption(replaceTags("Widget_type_propertes"));
	}

	void PanelProperties::shutdown()
	{
	}

	size_t PanelProperties::AddParametrs(WidgetStyle * widgetType, WidgetContainer * widgetContainer, int& y)
	{
		size_t count = widgetType->parameter.size();

		for (MyGUI::VectorStringPairs::iterator iter = widgetType->parameter.begin(); iter != widgetType->parameter.end(); ++iter)
		{
			std::string value = "";
			for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
			{
				if (iterProperty->first == iter->first)
				{
					value = iterProperty->second;
					break;
				}
			}
			eventCreatePair(mWidgetClient, iter->first, value, iter->second, y);
			y += PropertyItemHeight;
		}

		return count;
	}

	void PanelProperties::update(MyGUI::Widget* _current_widget, WidgetStyle* _widgetType)
	{
		if (_widgetType == nullptr)
		{
			setVisible(false);
			return;
		}

		int y = 0;

		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(_current_widget);

		MyGUI::LanguageManager::getInstance().addUserTag("widget_type", _widgetType->name);

		mPanelCell->setCaption(MyGUI::LanguageManager::getInstance().replaceTags("#{Widget_type_propertes}"));

		size_t count = AddParametrs(_widgetType, widgetContainer, y);

		setVisible( count > 0 );

		mPanelCell->setClientHeight(y);
	}

} // namespace tools
