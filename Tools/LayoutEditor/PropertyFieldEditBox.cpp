/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldEditBox.h"
#include "Common.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"
#include "Parse.h"
#include "GroupMessage.h"
#include "WidgetSelectorManager.h"
#include "SettingsManager.h"

namespace tools
{
	const std::string DEFAULT_STRING = "[DEFAULT]";

	PropertyFieldEditBox::PropertyFieldEditBox(MyGUI::Widget* _parent) :
		BaseLayout("PropertyFieldEditBox.layout", _parent),
		mText(nullptr),
		mField(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		mField->eventEditTextChange += newDelegate (this, &PropertyFieldEditBox::notifyTryApplyProperties);
		mField->eventEditSelectAccept += newDelegate (this, &PropertyFieldEditBox::notifyForceApplyProperties);
	}

	PropertyFieldEditBox::~PropertyFieldEditBox()
	{
	}

	void PropertyFieldEditBox::initialise(const std::string& _type, MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		mType = _type;
	}

	void PropertyFieldEditBox::notifyApplyProperties(MyGUI::Widget* _sender, bool _force)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		std::string value = mField->getOnlyText();
		if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
			value = "";

		onAction(value, _force);

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

	void PropertyFieldEditBox::onAction(const std::string& _value, bool _force)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		EditorWidgets* ew = &EditorWidgets::getInstance();
		WidgetContainer* widgetContainer = ew->find(mCurrentWidget);

		bool goodData = onCheckValue();

		if (goodData || _force)
		{
			bool success = ew->tryToApplyProperty(widgetContainer->widget, mName, _value);

			if (success)
			{
				// непонятно как сюда попало
				//EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");

				bool found = false;
				// если такое св-во было, то заменим (или удалим если стерли) значение
				for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
				{
					if (iterProperty->first == mName)
					{
						found = true;
						if (_value.empty())
							widgetContainer->mProperty.erase(iterProperty);
						else
							iterProperty->second = _value;
					}
				}

				// если такого свойства не было раньше, то сохраняем
				if (!_value.empty() && !found)
					widgetContainer->mProperty.push_back(MyGUI::PairString(mName, _value));
			}
			else
			{
				mField->setCaption(DEFAULT_VALUE);
			}
		}
	}

	void PropertyFieldEditBox::notifyTryApplyProperties(MyGUI::EditBox* _sender)
	{
		notifyApplyProperties(_sender, false);
	}

	void PropertyFieldEditBox::notifyForceApplyProperties(MyGUI::EditBox* _sender)
	{
		notifyApplyProperties(_sender, true);
	}

	bool PropertyFieldEditBox::onCheckValue()
	{
		bool success = true;
		return success;
	}

	MyGUI::IntSize PropertyFieldEditBox::getContentSize()
	{
		return MyGUI::IntSize(0, SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("PropertyItemHeight"));
	}

	void PropertyFieldEditBox::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

	void PropertyFieldEditBox::setValue(const std::string& _value)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		if (_value.empty())
		{
			mField->setCaption(DEFAULT_VALUE);
		}
		else
		{
			mField->setOnlyText(_value);
			onCheckValue();
		}
	}

	void PropertyFieldEditBox::setName(const std::string& _value)
	{
		mName = _value;
		mText->setCaption(_value);
	}

} // namespace tools
