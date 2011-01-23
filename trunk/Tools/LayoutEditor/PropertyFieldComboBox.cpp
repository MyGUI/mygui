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
		BaseLayout("PropertyFieldComboBox.layout", _parent),
		mText(nullptr),
		mField(nullptr),
		mCurrentWidget(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		mField->eventComboAccept += newDelegate (this, &PropertyFieldComboBox::notifyForceApplyProperties2);
	}

	PropertyFieldComboBox::~PropertyFieldComboBox()
	{
	}

	void PropertyFieldComboBox::initialise(const std::string& _type, MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		mType = _type;

		onFillValues();
	}

	void PropertyFieldComboBox::onFillValues()
	{
		WidgetStyle::VectorString values = WidgetTypes::getInstance().findPossibleValues(mType);

		for (WidgetStyle::VectorString::iterator iter = values.begin(); iter != values.end(); ++iter)
			mField->addItem(*iter);
		mField->beginToItemFirst();
	}

	void PropertyFieldComboBox::notifyApplyProperties(MyGUI::Widget* _sender)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;
		std::string value = mField->getOnlyText();

		if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
			value = "";

		onAction(value, true);
	}

	void PropertyFieldComboBox::onAction(const std::string& _value, bool _final)
	{
		eventAction(mName, _value, _final);
	}

	void PropertyFieldComboBox::notifyForceApplyProperties2(MyGUI::ComboBox* _sender, size_t _index)
	{
		notifyApplyProperties(_sender);
	}

	MyGUI::IntSize PropertyFieldComboBox::getContentSize()
	{
		return MyGUI::IntSize(0, mMainWidget->getHeight());
	}

	void PropertyFieldComboBox::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
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
		mName = _value;
		mText->setCaption(_value);
	}

} // namespace tools
