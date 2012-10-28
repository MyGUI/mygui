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
		mColourPlace(nullptr),
		mCurrentWidget(nullptr),
		mColourPanel(nullptr),
		mGoodData(false)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");
		assignWidget(mColourPlace, "ColourPlace");

		mField->eventEditTextChange += newDelegate (this, &PropertyFieldColour::notifyTryApplyProperties);
		mField->eventEditSelectAccept += newDelegate (this, &PropertyFieldColour::notifyForceApplyProperties);

		mColourPlace->eventMouseButtonPressed += MyGUI::newDelegate(this, &PropertyFieldColour::notifyMouseButtonPressed);

		mColourPanel = new ColourPanel();
		mColourPanel->Initialise();
		mColourPanel->eventEndDialog.connect(this, &PropertyFieldColour::notifyEndDialog);
		mColourPanel->eventPreviewColour.connect(this, &PropertyFieldColour::notifyPreviewColour);
	}

	PropertyFieldColour::~PropertyFieldColour()
	{
		delete mColourPanel;
		mColourPanel = nullptr;
	}

	void PropertyFieldColour::initialise(const std::string& _type)
	{
		mType = _type;
	}

	void PropertyFieldColour::setTarget(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
	}

	void PropertyFieldColour::notifyApplyProperties(MyGUI::Widget* _sender, bool _force)
	{
		bool goodData = onCheckValue();

		if (goodData || _force)
		{
			std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

			std::string value = mField->getOnlyText();
			if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
				value = "";

			onAction(value, true);
		}
	}

	void PropertyFieldColour::onAction(const std::string& _value, bool _final)
	{
		eventAction(mName, _value, _final);
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
		bool success = utility::checkParse<float>(mField, 3);

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

	void PropertyFieldColour::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		showColourDialog();
	}

	void PropertyFieldColour::notifyPreviewColour(const MyGUI::Colour& _value)
	{
		setColour(_value, false);
	}

	void PropertyFieldColour::notifyEndDialog(Dialog* _sender, bool _result)
	{
		mColourPanel->endModal();

		if (_result)
		{
			setColour(mColourPanel->getColour(), true);
		}
		else
		{
			if (mGoodData)
			{
				setColour(mPreviewColour, true);
			}
			else
			{
				std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;
				mField->setCaption(DEFAULT_VALUE);
				updateColourPlace(false);
				onAction("", true);
			}
		}
	}

	void PropertyFieldColour::showColourDialog()
	{
		mGoodData = onCheckValue();
		if (mGoodData)
			mPreviewColour = getColour();
		else
			mPreviewColour = MyGUI::Colour::White;

		mColourPanel->setColour(mPreviewColour);
		mColourPanel->setAlphaSupport(false);
		mColourPanel->doModal();
	}

	void PropertyFieldColour::setColour(const MyGUI::Colour& _color, bool _final)
	{
		std::string value = MyGUI::utility::toString(_color.red, " ", _color.green, " ", _color.blue);
		mField->setOnlyText(value);
		onCheckValue();
		onAction(value, _final);
	}

	MyGUI::Colour PropertyFieldColour::getColour()
	{
		return MyGUI::Colour::parse(mField->getOnlyText());
	}

	void PropertyFieldColour::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	bool PropertyFieldColour::getVisible()
	{
		return mMainWidget->getVisible();
	}

}
