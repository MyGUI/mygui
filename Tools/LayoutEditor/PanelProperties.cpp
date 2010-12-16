/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "Localise.h"
#include "PanelProperties.h"
#include "EditorWidgets.h"
#include "PropertyFieldManager.h"

namespace tools
{
	PanelProperties::PanelProperties() :
		BasePanelViewItem("PanelProperties.layout"),
		mDeep(0)
	{
	}

	void PanelProperties::initialise()
	{
		mPanelCell->setCaption(replaceTags("Widget_type_propertes"));
	}

	void PanelProperties::shutdown()
	{
		destroyPropertyFields();
	}

	void PanelProperties::setDeep(size_t _value)
	{
		mDeep = _value;
	}

	size_t PanelProperties::getDeep() const
	{
		return mDeep;
	}

	void PanelProperties::AddParametrs(WidgetStyle* widgetType, WidgetContainer* widgetContainer, MyGUI::Widget* _currentWidget)
	{
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

			PropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, iter->first, value, iter->second, _currentWidget);
			mFields.push_back(field);
		}
	}

	void PanelProperties::update(MyGUI::Widget* _currentWidget, WidgetStyle* _widgetType)
	{
		destroyPropertyFields();

		if (_widgetType == nullptr)
		{
			setVisible(false);
			return;
		}

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_currentWidget);

		MyGUI::LanguageManager::getInstance().addUserTag("widget_type", _widgetType->name);

		mPanelCell->setCaption(MyGUI::LanguageManager::getInstance().replaceTags("#{Widget_type_propertes}"));

		AddParametrs(_widgetType, widgetContainer, _currentWidget);

		bool visible = mFields.size() > 0;
		setVisible(visible);

		updateSize();
	}

	void PanelProperties::updateSize()
	{
		int height = 0;

		for (VectorPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
		{
			MyGUI::IntSize size = (*item)->getContentSize();
			(*item)->setCoord(MyGUI::IntCoord(0, height, mMainWidget->getWidth(), size.height));
			height += size.height;
		}

		mPanelCell->setClientHeight(height);
	}

	void PanelProperties::destroyPropertyFields()
	{
		for (VectorPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
			delete (*item);
		mFields.clear();
	}

} // namespace tools
