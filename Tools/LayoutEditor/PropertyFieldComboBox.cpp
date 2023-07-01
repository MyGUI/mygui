/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldComboBox.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"

namespace tools
{

	const std::string DEFAULT_STRING = "[DEFAULT]";

	PropertyFieldComboBox::PropertyFieldComboBox(MyGUI::Widget* _parent) :
		BaseLayout("PropertyFieldComboBox.layout", _parent)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		mField->eventComboAccept += newDelegate(this, &PropertyFieldComboBox::notifyForceApplyProperties2);
	}

	void PropertyFieldComboBox::initialise(std::string_view _type)
	{
		mType = _type;
	}

	void PropertyFieldComboBox::setTarget(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		onFillValues();
	}

	void PropertyFieldComboBox::onFillValues()
	{
		WidgetStyle::VectorString values = WidgetTypes::getInstance().findPossibleValues(mType);

		mField->removeAllItems();
		for (auto& value : values)
			mField->addItem(MyGUI::LanguageManager::getInstance().replaceTags(value));
		mField->beginToItemFirst();
	}

	void PropertyFieldComboBox::notifyApplyProperties(MyGUI::Widget* _sender)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;
		std::string value = mField->getOnlyText();

		if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
			value.clear();

		onAction(value, true);
	}

	void PropertyFieldComboBox::onAction(std::string_view _value, bool _final)
	{
		eventAction(mName, _value, _final);
	}

	void PropertyFieldComboBox::notifyForceApplyProperties2(MyGUI::ComboBox* _sender, size_t _index)
	{
		notifyApplyProperties(_sender);
	}

	MyGUI::IntSize PropertyFieldComboBox::getContentSize()
	{
		return {0, mMainWidget->getHeight()};
	}

	void PropertyFieldComboBox::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

	void PropertyFieldComboBox::setValue(std::string_view _value)
	{
		if (_value.empty())
		{
			mField->setCaption(replaceTags("ColourDefault") + DEFAULT_STRING);
		}
		else
		{
			mField->setOnlyText(MyGUI::UString(_value));
		}
	}

	void PropertyFieldComboBox::setName(std::string_view _value)
	{
		mName = _value;
		mText->setCaption(mName);
	}

	void PropertyFieldComboBox::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	bool PropertyFieldComboBox::getVisible()
	{
		return mMainWidget->getVisible();
	}

}
