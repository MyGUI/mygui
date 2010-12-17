/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldComboBox.h"
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

	PropertyFieldComboBox::PropertyFieldComboBox() :
		mText(nullptr),
		mField(nullptr)
	{
	}

	PropertyFieldComboBox::~PropertyFieldComboBox()
	{
		destroy();
	}

	void PropertyFieldComboBox::initialise(MyGUI::Widget* _window, const std::string& _type, MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		mType = _type;

		mText = _window->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(), MyGUI::Align::Default);
		mText->setTextAlign(MyGUI::Align::Right);

		mField = _window->createWidget<MyGUI::ComboBox>("ComboBox", MyGUI::IntCoord(), MyGUI::Align::Top | MyGUI::Align::HStretch);
		mField->eventComboAccept += newDelegate (this, &PropertyFieldComboBox::notifyForceApplyProperties2);

		mField->setComboModeDrop(true);

		mField->setNeedToolTip(true);
		mField->eventToolTip += newDelegate (this, &PropertyFieldComboBox::notifyToolTip);

		onFillValues();
	}

	void PropertyFieldComboBox::onFillValues()
	{
		WidgetStyle::VectorString values = WidgetTypes::getInstance().findPossibleValues(mType);

		for (WidgetStyle::VectorString::iterator iter = values.begin(); iter != values.end(); ++iter)
			mField->addItem(*iter);
		mField->beginToItemFirst();
	}

	void PropertyFieldComboBox::destroy()
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(mText);
		mText = nullptr;
		MyGUI::WidgetManager::getInstance().destroyWidget(mField);
		mField = nullptr;
	}

	void PropertyFieldComboBox::notifyApplyProperties(MyGUI::Widget* _sender)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;
		std::string value = mField->getOnlyText();

		if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
			value = "";

		onAction(value);
	}

	void PropertyFieldComboBox::onAction(const std::string& _value)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		std::string action = mField->getUserString("action");

		bool success = EditorWidgets::getInstance().tryToApplyProperty(widgetContainer->widget, action, _value);

		bool found = false;
		// если такое св-во было, то заменим (или удалим если стерли) значение
		for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
		{
			if (iterProperty->first == action)
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
			widgetContainer->mProperty.push_back(MyGUI::PairString(action, _value));

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

	void PropertyFieldComboBox::notifyForceApplyProperties2(MyGUI::ComboBox* _sender, size_t _index)
	{
		notifyApplyProperties(_sender);
	}

	void PropertyFieldComboBox::onToolTip(const MyGUI::ToolTipInfo& _info)
	{
	}

	void PropertyFieldComboBox::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
	{
		onToolTip(_info);
	}

	MyGUI::IntSize PropertyFieldComboBox::getContentSize()
	{
		return MyGUI::IntSize(0, SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("PropertyItemHeight"));
	}

	void PropertyFieldComboBox::setCoord(const MyGUI::IntCoord& _coord)
	{
		int w1 = 120;
		int x2 = 125;

		mText->setCoord(MyGUI::IntCoord(0, _coord.top, w1, _coord.height));
		mField->setCoord(MyGUI::IntCoord(x2, _coord.top, _coord.width - x2, _coord.height));
	}

	void PropertyFieldComboBox::setValue(const std::string& _value)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		if (_value.empty())
		{
			mField->setCaption(DEFAULT_VALUE);
		}
		else
		{
			mField->setOnlyText(_value);
		}
	}

	void PropertyFieldComboBox::setName(const std::string& _value)
	{
		mField->setUserString("action", _value);
		mText->setCaption(_value);
	}

} // namespace tools
