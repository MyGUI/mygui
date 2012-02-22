/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "PropertyColourControl.h"
#include "Localise.h"

namespace tools
{

	PropertyColourControl::PropertyColourControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("PropertyColourControl.layout", _parent),
		mEdit(nullptr),
		mColour(nullptr),
		mColourPanel(nullptr)
	{
		assignWidget(mEdit, "Edit");
		assignWidget(mColour, "Colour");

		mEdit->eventEditTextChange += MyGUI::newDelegate(this, &PropertyColourControl::notifyEditTextChange);
		mColour->eventMouseButtonClick += MyGUI::newDelegate(this, &PropertyColourControl::notifyMouseButtonClick);

		mColourPanel = new ColourPanel();
		mColourPanel->setAlphaSupport(false);
		mColourPanel->eventEndDialog = MyGUI::newDelegate(this, &PropertyColourControl::notifyEndDialog);
		mColourPanel->eventPreviewColour = MyGUI::newDelegate(this, &PropertyColourControl::notifyPreviewColour);
	}

	PropertyColourControl::~PropertyColourControl()
	{
		delete mColourPanel;
		mColourPanel = nullptr;

		mColour->eventMouseButtonClick -= MyGUI::newDelegate(this, &PropertyColourControl::notifyMouseButtonClick);
		mEdit->eventEditTextChange -= MyGUI::newDelegate(this, &PropertyColourControl::notifyEditTextChange);
	}

	void PropertyColourControl::updateProperty()
	{
		Property* proper = getProperty();
		if (proper != nullptr)
		{
			mEdit->setEnabled(!proper->getReadOnly());
			mEdit->setCaption(proper->getValue());

			MyGUI::Colour colour;
			bool validate = isValidate(colour);
			if (validate)
			{
				mCurrentColour = colour;
				mColour->setColour(mCurrentColour);
				mColour->setAlpha(mCurrentColour.alpha);
			}

			setColour(validate);
		}
		else
		{
			mEdit->setCaption("");
			mEdit->setEnabled(false);

			mCurrentColour = MyGUI::Colour::Zero;
			mColour->setAlpha(mCurrentColour.alpha);
		}
	}

	void PropertyColourControl::notifyEditTextChange(MyGUI::EditBox* _sender)
	{
		Property* proper = getProperty();
		if (proper != nullptr)
		{
			MyGUI::Colour colour;
			bool validate = isValidate(colour);
			if (validate)
			{
				proper->setValue(getClearValue(), getTypeName());

				mCurrentColour = colour;
				mColour->setColour(mCurrentColour);
				mColour->setAlpha(mCurrentColour.alpha);
			}
			else
			{
				mCurrentColour = MyGUI::Colour::Zero;
				mColour->setAlpha(mCurrentColour.alpha);
			}

			setColour(validate);
		}
	}

	bool PropertyColourControl::isValidate(MyGUI::Colour& _resultValue)
	{
		MyGUI::UString value = mEdit->getOnlyText();

		if (parseColour1(value, _resultValue))
			return true;
		if (parseColour2(value, _resultValue))
			return true;
		//if (parseColour3(value, _resultValue))
		//	return true;

		return false;
	}

	MyGUI::UString PropertyColourControl::getClearValue()
	{
		MyGUI::UString value = mEdit->getOnlyText();

		return value;
	}

	void PropertyColourControl::setColour(bool _validate)
	{
		MyGUI::UString value = mEdit->getOnlyText();

		size_t index = mEdit->getTextCursor();

		mEdit->setOnlyText(value);
		if (!_validate)
		{
			mEdit->setCaption(replaceTags("ColourError") + mEdit->getOnlyText());
		}

		mEdit->setTextCursor(index);
	}

	bool PropertyColourControl::parseColour1(const std::string& _value, MyGUI::Colour& _resultValue)
	{
		if (!_value.empty())
		{
			if (_value[0] == '#')
			{
				std::istringstream stream(_value.substr(1));
				int result = 0;
				stream >> std::hex >> result;
				if (!stream.fail())
				{
					int item = stream.get();
					while (item != -1)
					{
						if (item != ' ' && item != '\t')
							return false;
						item = stream.get();
					}

					_resultValue = MyGUI::Colour( (unsigned char)( result >> 16 ) / 256.0f, (unsigned char)( result >> 8 ) / 256.0f, (unsigned char)( result ) / 256.0f );
					return true;
				}
			}
		}

		return false;
	}

	bool PropertyColourControl::parseColour2(const std::string& _value, MyGUI::Colour& _resultValue)
	{
		if (!_value.empty())
		{
			float red, green, blue;
			std::istringstream stream(_value);
			stream >> red >> green >> blue;
			if (!stream.fail())
			{
				int item = stream.get();
				while (item != -1)
				{
					if (item != ' ' && item != '\t')
						return false;
					item = stream.get();
				}

				_resultValue = MyGUI::Colour(red, green, blue);
				return true;
			}
		}

		return false;
	}

	bool PropertyColourControl::parseColour3(const std::string& _value, MyGUI::Colour& _resultValue)
	{
		if (!_value.empty())
		{
			float red, green, blue, alpha;
			std::istringstream stream(_value);
			stream >> red >> green >> blue >> alpha;
			if (!stream.fail())
			{
				int item = stream.get();
				while (item != -1)
				{
					if (item != ' ' && item != '\t')
						return false;
					item = stream.get();
				}

				_resultValue = MyGUI::Colour(red, green, blue, alpha);
				return true;
			}
		}

		return false;
	}

	void PropertyColourControl::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		mPreviewColour = mCurrentColour;
		mColourPanel->setColour(mCurrentColour);
		mColourPanel->doModal();
	}

	void PropertyColourControl::notifyEndDialog(Dialog* _sender, bool _result)
	{
		mColourPanel->endModal();

		if (_result)
		{
			mCurrentColour = mColourPanel->getColour();
			mCurrentColour.alpha = 1;

			updateSetProperty();
		}
		else
		{
			mCurrentColour = mPreviewColour;

			updateSetProperty();
		}
	}

	void PropertyColourControl::notifyPreviewColour(const MyGUI::Colour& _value)
	{
		mCurrentColour = _value;
		mCurrentColour.alpha = 1;

		updateSetProperty();
	}

	void PropertyColourControl::updateSetProperty()
	{
		Property* proper = getProperty();
		if (proper != nullptr)
			proper->setValue(MyGUI::utility::toString(mCurrentColour.red, " ", mCurrentColour.green, " ", mCurrentColour.blue), ""); // чтобы мы обновили поле
	}

} // namespace tools
