/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldColour.h"
#include "Localise.h"
#include "UndoManager.h"
#include "Parse.h"

namespace tools
{
	const std::string DEFAULT_STRING = "[DEFAULT]";

	PropertyFieldColour::PropertyFieldColour(MyGUI::Widget* _parent) :
		BaseLayout("PropertyFieldColour.layout", _parent),
		mText(nullptr),
		mField(nullptr),
		mColourPlace(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");
		assignWidget(mColourPlace, "ColourPlace");

		mField->eventEditTextChange += newDelegate (this, &PropertyFieldColour::notifyTryApplyProperties);
		mField->eventEditSelectAccept += newDelegate (this, &PropertyFieldColour::notifyForceApplyProperties);
	}

	PropertyFieldColour::~PropertyFieldColour()
	{
	}

	void PropertyFieldColour::initialise(const std::string& _type, MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		mType = _type;
	}

	void PropertyFieldColour::notifyApplyProperties(MyGUI::Widget* _sender, bool _force)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		std::string value = mField->getOnlyText();
		if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
			value = "";

		bool goodData = onCheckValue();

		if (goodData || _force)
		{
			onAction(value);
		}
	}

	void PropertyFieldColour::onAction(const std::string& _value)
	{
		eventAction(mName, _value);
	}

	void PropertyFieldColour::notifyTryApplyProperties(MyGUI::EditBox* _sender)
	{
		notifyApplyProperties(_sender, false);
	}

	void PropertyFieldColour::notifyForceApplyProperties(MyGUI::EditBox* _sender)
	{
		notifyApplyProperties(_sender, true);
	}

	bool PropertyFieldColour::onCheckValue()
	{
		bool success = true;

		success = utility::checkParse<float>(mField, 3);

		updateColourPlace(success);

		return success;
	}

	void PropertyFieldColour::updateColourPlace(bool _success)
	{
		if (_success)
		{
			mColourPlace->setAlpha(1);
			MyGUI::Colour colour = MyGUI::Colour::parse(mField->getOnlyText());
			mColourPlace->setColour(colour);
		}
		else
		{
			mColourPlace->setAlpha(0);
		}
	}

	MyGUI::IntSize PropertyFieldColour::getContentSize()
	{
		return MyGUI::IntSize(0, mMainWidget->getHeight());
	}

	void PropertyFieldColour::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

	void PropertyFieldColour::setValue(const std::string& _value)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		if (_value.empty())
		{
			mField->setCaption(DEFAULT_VALUE);
			updateColourPlace(false);
		}
		else
		{
			mField->setOnlyText(_value);
			onCheckValue();
		}
	}

	void PropertyFieldColour::setName(const std::string& _value)
	{
		mName = _value;
		mText->setCaption(_value);
	}

} // namespace tools
