/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "PanelProperties.h"
#include "Localise.h"
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

	size_t PanelProperties::addParametrs(WidgetStyle* widgetType, WidgetContainer* widgetContainer, MyGUI::Widget* _currentWidget)
	{
		size_t result = 0;

		for (MyGUI::VectorStringPairs::iterator iter = widgetType->parameter.begin(); iter != widgetType->parameter.end(); ++iter)
		{
			std::string value = widgetContainer->getProperty(iter->first);

			IPropertyField* field = getPropertyField(mWidgetClient, iter->first, iter->second);
			field->setTarget(_currentWidget);
			field->setValue(value);

			result ++;
		}

		return result;
	}

	void PanelProperties::update(MyGUI::Widget* _currentWidget, WidgetStyle* _widgetType)
	{
		hidePropertyFields();

		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_currentWidget);

		MyGUI::LanguageManager::getInstance().addUserTag("widget_type", _widgetType->name);

		mPanelCell->setCaption(replaceTags("Widget_type_propertes"));

		size_t count = addParametrs(_widgetType, widgetContainer, mCurrentWidget);

		setVisible(count > 0);

		updateSize();
	}

	void PanelProperties::updateSize()
	{
		int height = 0;

		for (MapPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
		{
			if ((*item).second->getVisible())
			{
				MyGUI::IntSize size = (*item).second->getContentSize();
				(*item).second->setCoord(MyGUI::IntCoord(0, height, mMainWidget->getWidth(), size.height));
				height += size.height;
			}
		}

		mPanelCell->setClientHeight(height);
	}

	void PanelProperties::destroyPropertyFields()
	{
		for (MapPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
			delete (*item).second;
		mFields.clear();
	}

	void PanelProperties::notifyAction(const std::string& _name, const std::string& _value, bool _final)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		EditorWidgets::getInstance().tryToApplyProperty(widgetContainer->getWidget(), _name, _value);

		if (_final)
		{
			if (_value.empty())
				widgetContainer->clearProperty(_name);
			else
				widgetContainer->setProperty(_name, _value);

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

	void PanelProperties::hidePropertyFields()
	{
		for (MapPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
			(*item).second->setVisible(false);
	}

	IPropertyField* PanelProperties::getPropertyField(MyGUI::Widget* _client, const std::string& _name, const std::string& _type)
	{
		MapPropertyField::iterator item = mFields.find(_name);
		if (item != mFields.end())
		{
			(*item).second->setVisible(true);
			return (*item).second;
		}

		IPropertyField* result = PropertyFieldManager::getInstance().createPropertyField(_client, _type);
		result->setName(_name);
		result->eventAction = MyGUI::newDelegate(this, &PanelProperties::notifyAction);

		mFields[_name] = result;

		return result;
	}

}
