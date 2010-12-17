/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "PanelMainProperties.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "PropertyFieldManager.h"

namespace tools
{

	PanelMainProperties::PanelMainProperties() :
		BasePanelViewItem("PanelMainProperties.layout"),
		mCurrentWidget(nullptr)
	{
	}

	void PanelMainProperties::initialise()
	{
		mPanelCell->setCaption(replaceTags("Main_properties"));
	}

	void PanelMainProperties::shutdown()
	{
		destroyPropertyFields();
	}

	void PanelMainProperties::update(MyGUI::Widget* _currentWidget)
	{
		destroyPropertyFields();

		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName());
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		IPropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Name", _currentWidget);
		field->setName("Name");
		field->setValue(widgetContainer->name);
		mFields.push_back(field);

		if (widgetType->resizeable)
		{
			field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Position", _currentWidget);
			field->setName("Position");
			field->setValue(widgetContainer->position());
			mFields.push_back(field);
		}

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Type", _currentWidget);
		field->setName("Type");
		field->setValue(widgetContainer->type);
		mFields.push_back(field);

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Align", _currentWidget);
		field->setName("Align");
		field->setValue(widgetContainer->align);
		mFields.push_back(field);

		if (mCurrentWidget->isRootWidget())
		{
			IPropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Layer", _currentWidget);
			field->setName("Layer");
			field->setValue(widgetContainer->getLayerName());
			mFields.push_back(field);
		}

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Skin", _currentWidget);
		field->setName("Skin");
		field->setValue(widgetContainer->skin);
		mFields.push_back(field);

		updateSize();
	}

	void PanelMainProperties::updateSize()
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

	void PanelMainProperties::destroyPropertyFields()
	{
		for (VectorPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
			delete (*item);
		mFields.clear();
	}

} // namespace tools
