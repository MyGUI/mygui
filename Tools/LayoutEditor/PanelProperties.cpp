/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/

#include "precompiled.h"
#include "PanelProperties.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"

inline const Ogre::UTFString localise(const Ogre::UTFString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

PanelProperties::PanelProperties() : BasePanelViewItem("PanelProperties.layout")
{
}

void PanelProperties::initialise()
{
	mPanelCell->setCaption(localise("Widget_type_propertes"));
}

void PanelProperties::shutdown()
{
}

void PanelProperties::update(MyGUI::WidgetPtr _current_widget, PropertiesGroup _group)
{
	int y = 0;

	WidgetStyle * widgetType = WidgetTypes::getInstance().find(_current_widget->getTypeName());
	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(_current_widget);

	if (_group == TYPE_PROPERTIES)
	{
		MyGUI::LanguageManager::getInstance().addUserTag("widget_type", _current_widget->getTypeName());
		if (widgetType->name == "Widget")
		{
			if (_current_widget->getTypeName() != "Widget")
			{
				mPanelCell->setCaption(MyGUI::LanguageManager::getInstance().replaceTags(localise("Properties_not_available")));
				y += PropertyItemHeight;
			}
			else
			{
				setVisible(false);
			}
		}
		else
		{
			mPanelCell->setCaption(MyGUI::LanguageManager::getInstance().replaceTags(localise("Widget_type_propertes")));

			for (StringPairs::iterator iter = widgetType->parameter.begin(); iter != widgetType->parameter.end(); ++iter)
			{
				std::string value = "";
				for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
					if (iterProperty->first == iter->first){ value = iterProperty->second; break;}
				eventCreatePair(mWidgetClient, iter->first, value, iter->second, y);
				y += PropertyItemHeight;
			}
			setVisible( ! widgetType->parameter.empty() );
		}
	}
	else if (_group == WIDGET_PROPERTIES || _group == EVENTS)
	{
		if (_group == WIDGET_PROPERTIES) mPanelCell->setCaption(localise("Other_properties"));
		else mPanelCell->setCaption(localise("Events"));

		if (_current_widget->getTypeName() != "Sheet" &&
			_current_widget->getTypeName() != MyGUI::TabItem::getClassTypeName())
		{
			setVisible(true);
			//base properties (from Widget)
			WidgetStyle * baseType = WidgetTypes::getInstance().find("Widget");
			for (StringPairs::iterator iter = baseType->parameter.begin(); iter != baseType->parameter.end(); ++iter)
			{
				std::string value = "";
				for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
					if (iterProperty->first == iter->first){ value = iterProperty->second; break;}
				if ((0 == strncmp("Widget_event", iter->first.c_str(), 12)) ^ (_group == WIDGET_PROPERTIES))
				{
					eventCreatePair(mWidgetClient, iter->first, value, iter->second, y);
					y += PropertyItemHeight;
				}
			}
		}
		else
		{
			setVisible(false);
		}
	}

	mPanelCell->setClientHeight(y, true);
}
