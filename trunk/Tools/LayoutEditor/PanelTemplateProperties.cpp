/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PanelTemplateProperties.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "PropertyFieldManager.h"
#include "UndoManager.h"

namespace tools
{

	PanelTemplateProperties::PanelTemplateProperties() :
		BasePanelViewItem("PanelTemplateProperties.layout"),
		mCurrentWidget(nullptr)
	{
	}

	void PanelTemplateProperties::initialise()
	{
		mPanelCell->setCaption(replaceTags("PanelTemplatePropertiesName"));
	}

	void PanelTemplateProperties::shutdown()
	{
		destroyPropertyFields();
	}

	void PanelTemplateProperties::AddParametrs(WidgetStyle* widgetType, WidgetContainer* widgetContainer, MyGUI::Widget* _currentWidget)
	{
		if (widgetType != nullptr)
		{
			for (MyGUI::VectorStringPairs::iterator iter = widgetType->templateData.begin(); iter != widgetType->templateData.end(); ++iter)
			{
				std::string value = widgetContainer->getUserData(iter->first);

				IPropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, iter->second);
				field->setTarget(_currentWidget);
				field->setName(iter->first);
				field->setValue(value);
				field->eventAction = MyGUI::newDelegate(this, &PanelTemplateProperties::notifyAction);
				mFields.push_back(field);
			}
		}
	}

	void PanelTemplateProperties::update(MyGUI::Widget* _currentWidget, WidgetStyle* _widgetType)
	{
		destroyPropertyFields();

		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_currentWidget);

		//mPanelCell->setCaption(replaceTags("PanelTemplatePropertiesName"));

		AddParametrs(_widgetType, widgetContainer, mCurrentWidget);

		bool visible = !mFields.empty();
		setVisible(visible);

		updateSize();
	}

	void PanelTemplateProperties::updateSize()
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

	void PanelTemplateProperties::destroyPropertyFields()
	{
		for (VectorPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
			delete (*item);
		mFields.clear();
	}

	void PanelTemplateProperties::notifyAction(const std::string& _name, const std::string& _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			widgetContainer->setUserData(_name, _value);

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

}
