/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldEditBoxAccept.h"
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
	#define ON_EXIT( CODE ) class _OnExit { public: void dummy() const { }; ~_OnExit() { CODE; } } _onExit; _onExit.dummy()

	const std::string DEFAULT_STRING = "[DEFAULT]";
	const int BAR_HEIGHT = 30;

	PropertyFieldEditBoxAccept::PropertyFieldEditBoxAccept() :
		mText(nullptr),
		mField(nullptr)
	{
	}

	PropertyFieldEditBoxAccept::~PropertyFieldEditBoxAccept()
	{
		destroy();
	}

	void PropertyFieldEditBoxAccept::initialise(MyGUI::Widget* _window, const std::string& _type, MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		mType = _type;

		mText = _window->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(), MyGUI::Align::Default);
		mText->setTextAlign(MyGUI::Align::Right);

		mField = _window->createWidget<MyGUI::EditBox>("Edit", MyGUI::IntCoord(), MyGUI::Align::Top | MyGUI::Align::HStretch);
		mField->castType<MyGUI::EditBox>()->eventEditSelectAccept += newDelegate (this, &PropertyFieldEditBoxAccept::notifyForceApplyProperties);

		mField->setUserString("type", mType);
	}

	void PropertyFieldEditBoxAccept::destroy()
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(mText);
		mText = nullptr;
		MyGUI::WidgetManager::getInstance().destroyWidget(mField);
		mField = nullptr;
	}

	void PropertyFieldEditBoxAccept::notifyApplyProperties(MyGUI::Widget* _sender, bool _force)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		EditorWidgets* ew = &EditorWidgets::getInstance();
		WidgetContainer* widgetContainer = ew->find(mCurrentWidget);
		MyGUI::EditBox* senderEdit = _sender->castType<MyGUI::EditBox>();
		std::string action = senderEdit->getUserString("action");
		std::string value = senderEdit->getOnlyText();
		std::string type = senderEdit->getUserString("type");

		ON_EXIT(UndoManager::getInstance().addValue(PR_PROPERTIES););

		bool goodData = checkType(senderEdit, type);

		if (value == DEFAULT_STRING && senderEdit->getCaption() == DEFAULT_VALUE)
			value = "";

		bool success = false;
		if (goodData || _force)
			success = ew->tryToApplyProperty(widgetContainer->widget, action, value);
		else
			return;

		if (success)
		{
			EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");
		}
		else
		{
			senderEdit->setCaption(DEFAULT_VALUE);
			return;
		}

		// если такое св-во было, то заменим (или удалим если стерли) значение
		for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
		{
			if (iterProperty->first == action)
			{
				if (value.empty())
					widgetContainer->mProperty.erase(iterProperty);
				else
					iterProperty->second = value;
				return;
			}
		}

		// если такого свойства не было раньше, то сохраняем
		if (!value.empty())
			widgetContainer->mProperty.push_back(MyGUI::PairString(action, value));
	}

	void PropertyFieldEditBoxAccept::notifyForceApplyProperties(MyGUI::EditBox* _sender)
	{
		notifyApplyProperties(_sender, true);
	}

	bool PropertyFieldEditBoxAccept::checkType(MyGUI::EditBox* _edit, const std::string& _type)
	{
		bool success = true;
		return success;
	}

	MyGUI::IntSize PropertyFieldEditBoxAccept::getContentSize()
	{
		return MyGUI::IntSize(0, SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("PropertyItemHeight"));
	}

	void PropertyFieldEditBoxAccept::setCoord(const MyGUI::IntCoord& _coord)
	{
		int w1 = 120;
		int x2 = 125;

		mText->setCoord(MyGUI::IntCoord(0, _coord.top, w1, _coord.height));
		mField->setCoord(MyGUI::IntCoord(x2, _coord.top, _coord.width - x2, _coord.height));
	}

	void PropertyFieldEditBoxAccept::setValue(const std::string& _value)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		if (_value.empty())
		{
			mField->castType<MyGUI::EditBox>()->setCaption(DEFAULT_VALUE);
		}
		else
		{
			mField->castType<MyGUI::EditBox>()->setOnlyText(_value);
			checkType(mField->castType<MyGUI::EditBox>(), mType);
		}
	}

	void PropertyFieldEditBoxAccept::setName(const std::string& _value)
	{
		mField->setUserString("action", _value);
		mText->setCaption(_value);
	}

} // namespace tools
