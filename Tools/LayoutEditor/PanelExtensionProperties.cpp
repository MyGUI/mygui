/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PanelExtensionProperties.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "PropertyFieldManager.h"
#include "UndoManager.h"

namespace tools
{
	PanelExtensionProperties::PanelExtensionProperties() :
		BasePanelViewItem("PanelExtensionProperties.layout"),
		mCurrentWidget(nullptr)
	{
	}

	void PanelExtensionProperties::initialise()
	{
		mPanelCell->setCaption(replaceTags("PanelExtensionPropertiesName"));
	}

	void PanelExtensionProperties::shutdown()
	{
		destroyPropertyFields();
	}

	void PanelExtensionProperties::AddParametrs(WidgetStyle* widgetType, WidgetContainer* widgetContainer, MyGUI::Widget* _currentWidget)
	{
		if (widgetType != nullptr)
		{
			for (MyGUI::VectorStringPairs::iterator iter = widgetType->parameterData.begin(); iter != widgetType->parameterData.end(); ++iter)
			{
				std::string value = "";
				for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mUserString.begin(); iterProperty != widgetContainer->mUserString.end(); ++iterProperty)
				{
					if (iterProperty->first == iter->first)
					{
						value = iterProperty->second;
						break;
					}
				}

				IPropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, iter->second, _currentWidget);
				field->setName(iter->first);
				field->setValue(value);
				field->eventAction = MyGUI::newDelegate(this, &PanelExtensionProperties::notifyAction);
				mFields.push_back(field);
			}
		}
	}

	void PanelExtensionProperties::update(MyGUI::Widget* _currentWidget)
	{
		destroyPropertyFields();

		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_currentWidget);
		WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(widgetContainer->type);

		AddParametrs(widgetType, widgetContainer, mCurrentWidget);

		bool visible = mFields.size() > 0;
		setVisible(visible);

		updateSize();
	}

	void PanelExtensionProperties::updateSize()
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

	void PanelExtensionProperties::destroyPropertyFields()
	{
		for (VectorPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
			delete (*item);
		mFields.clear();
	}

	void PanelExtensionProperties::notifyAction(const std::string& _name, const std::string& _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			widgetContainer->setUserData(_name, _value);

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

} // namespace tools
