/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldEditBox.h"
#include "Localise.h"
#include "UndoManager.h"

namespace tools
{

	const std::string DEFAULT_STRING = "[DEFAULT]";

	PropertyFieldEditBox::PropertyFieldEditBox(MyGUI::Widget* _parent) :
		BaseLayout("PropertyFieldEditBox.layout", _parent)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		mField->eventEditTextChange += newDelegate(this, &PropertyFieldEditBox::notifyTryApplyProperties);
		mField->eventEditSelectAccept += newDelegate(this, &PropertyFieldEditBox::notifyForceApplyProperties);
	}

	void PropertyFieldEditBox::initialise(std::string_view _type)
	{
		mType = _type;
	}

	void PropertyFieldEditBox::setTarget(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
	}

	void PropertyFieldEditBox::notifyApplyProperties(MyGUI::Widget* _sender, bool _force)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		std::string value = mField->getOnlyText();
		if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
			value.clear();

		bool goodData = onCheckValue();

		if (goodData || _force)
		{
			onAction(value, true);
		}
	}

	void PropertyFieldEditBox::onAction(std::string_view _value, bool _final)
	{
		eventAction(mName, _value, _final);
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
		return {0, mMainWidget->getHeight()};
	}

	void PropertyFieldEditBox::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

	void PropertyFieldEditBox::setValue(std::string_view _value)
	{
		if (_value.empty())
		{
			std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

			mField->setCaption(DEFAULT_VALUE);
		}
		else
		{
			mField->setOnlyText(MyGUI::UString(_value));
			onCheckValue();
		}
	}

	void PropertyFieldEditBox::setName(std::string_view _value)
	{
		mName = _value;
		mText->setCaption(mName);
	}

	void PropertyFieldEditBox::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	bool PropertyFieldEditBox::getVisible()
	{
		return mMainWidget->getVisible();
	}

}
