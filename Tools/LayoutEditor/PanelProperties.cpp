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
#include "UndoManager.h"

namespace tools
{
	PanelProperties::PanelProperties() :
		BasePanelViewItem("PanelProperties.layout"),
		mDeep(0),
		mCurrentWidget(nullptr)
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

			IPropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, iter->second, _currentWidget);
			field->setName(iter->first);
			field->setValue(value);
			field->eventAction = MyGUI::newDelegate(this, &PanelProperties::notifyAction);
			mFields.push_back(field);
		}
	}

	void PanelProperties::update(MyGUI::Widget* _currentWidget, WidgetStyle* _widgetType)
	{
		destroyPropertyFields();

		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_currentWidget);

		MyGUI::LanguageManager::getInstance().addUserTag("widget_type", _widgetType->name);

		mPanelCell->setCaption(replaceTags("Widget_type_propertes"));

		AddParametrs(_widgetType, widgetContainer, mCurrentWidget);

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

	void PanelProperties::notifyAction(const std::string& _name, const std::string& _value)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		bool success = EditorWidgets::getInstance().tryToApplyProperty(widgetContainer->widget, _name, _value);

		bool found = false;
		// если такое св-во было, то заменим (или удалим если стерли) значение
		for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
		{
			if (iterProperty->first == _name)
			{
				found = true;
				if (_value.empty())
					widgetContainer->mProperty.erase(iterProperty);
				else
					iterProperty->second = _value;
				break;
			}
		}

		// если такого свойства не было раньше, то сохраняем
		if (!_value.empty() && !found)
			widgetContainer->mProperty.push_back(MyGUI::PairString(_name, _value));

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

} // namespace tools
