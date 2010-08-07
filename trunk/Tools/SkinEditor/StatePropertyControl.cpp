/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "StatePropertyControl.h"
#include "SkinManager.h"

namespace tools
{

	StatePropertyControl::StatePropertyControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("StatePropertyControl.layout", _parent),
		mTypeName("StatePropertyControl"),
		mVisible(nullptr),
		mPosition(nullptr),
		mCurrentSkin(nullptr),
		mCurrentState(nullptr)
	{
		assignWidget(mVisible, "Visible");
		assignWidget(mPosition, "Position");

		mVisible->eventComboChangePosition += MyGUI::newDelegate(this, &StatePropertyControl::notifyComboChangePosition);
		mPosition->eventEditTextChange += MyGUI::newDelegate(this, &StatePropertyControl::notifyEditTextChange);

		fillVisibleBox();

		SkinManager::getInstance().eventChangeSelection += MyGUI::newDelegate(this, &StatePropertyControl::notifyChangeSelection);
		advice();

		updateAllProperties();
	}

	StatePropertyControl::~StatePropertyControl()
	{
		mVisible->eventComboChangePosition -= MyGUI::newDelegate(this, &StatePropertyControl::notifyComboChangePosition);
		mPosition->eventEditTextChange -= MyGUI::newDelegate(this, &StatePropertyControl::notifyEditTextChange);

		SkinManager::getInstance().eventChangeSelection -= MyGUI::newDelegate(this, &StatePropertyControl::notifyChangeSelection);
		unadvice();
	}

	void StatePropertyControl::fillVisibleBox()
	{
		mVisible->removeAllItems();
		mVisible->addItem("True");
		mVisible->addItem("False");
	}

	void StatePropertyControl::notifyChangeSelection()
	{
		unadvice();
		advice();

		updateAllProperties();
	}

	void StatePropertyControl::unadvice()
	{
		if (mCurrentSkin != nullptr)
		{
			unadviceState();

			mCurrentSkin->eventChangeSelection -= MyGUI::newDelegate(this, &StatePropertyControl::notifyStateChangeSelection);
			mCurrentSkin = nullptr;
		}
	}

	void StatePropertyControl::advice()
	{
		mCurrentSkin = SkinManager::getInstance().getItemSelected();

		if (mCurrentSkin != nullptr)
		{
			mCurrentSkin->eventChangeSelection += MyGUI::newDelegate(this, &StatePropertyControl::notifyStateChangeSelection);

			adviceState();
		}
	}

	void StatePropertyControl::adviceState()
	{
		mCurrentState = mCurrentSkin->getItemSelected();
		if (mCurrentState != nullptr)
		{
			mCurrentState->getPropertySet()->eventChangeProperty += MyGUI::newDelegate(this, &StatePropertyControl::notifyChangeProperty);
		}
	}

	void StatePropertyControl::unadviceState()
	{
		if (mCurrentState != nullptr)
		{
			mCurrentState->getPropertySet()->eventChangeProperty -= MyGUI::newDelegate(this, &StatePropertyControl::notifyChangeProperty);
			mCurrentState = nullptr;
		}
	}

	void StatePropertyControl::notifyStateChangeSelection()
	{
		unadviceState();
		adviceState();

		updateAllProperties();
	}

	void StatePropertyControl::notifyChangeProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Visible")
				updateVisible();
			else if (_sender->getName() == "Position")
				updatePosition();
		}
	}

	void StatePropertyControl::updateAllProperties()
	{
		updateVisible();
		updatePosition();
	}

	void StatePropertyControl::updateVisible()
	{
		size_t index = MyGUI::ITEM_NONE;

		if (mCurrentState != nullptr)
		{
			MyGUI::UString visible;

			Property* prop = mCurrentState->getPropertySet()->getChild("Visible");
			if (prop != nullptr)
				visible = prop->getValue();

			index = getComboIndex(visible);
		}

		mVisible->setIndexSelected(index);
	}

	size_t StatePropertyControl::getComboIndex(const MyGUI::UString& _name)
	{
		size_t result = MyGUI::ITEM_NONE;

		size_t count = mVisible->getItemCount();
		for (size_t index=0; index<count; ++index)
		{
			if (mVisible->getItemNameAt(index) == _name)
			{
				result = index;
				break;
			}
		}

		return result;
	}

	void StatePropertyControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		if (mCurrentState != nullptr)
		{
			Property* prop = mCurrentState->getPropertySet()->getChild("Visible");
			if (prop != nullptr)
			{
				if (_index != MyGUI::ITEM_NONE)
					prop->setValue(_sender->getItemNameAt(_index), mTypeName);
				else
					prop->setValue("", mTypeName);
			}
		}
	}

	void StatePropertyControl::updatePosition()
	{
		MyGUI::UString value;

		if (mCurrentState != nullptr)
		{
			Property* prop = mCurrentState->getPropertySet()->getChild("Position");
			if (prop != nullptr)
				value = prop->getValue();
		}

		mPosition->setCaption(value);

		bool validate = isPositionValidate();
		setColourPosition(validate);
	}

	void StatePropertyControl::notifyEditTextChange(MyGUI::Edit* _sender)
	{
		if (mCurrentState != nullptr)
		{
			Property* prop = mCurrentState->getPropertySet()->getChild("Position");
			if (prop != nullptr)
			{
				bool validate = isPositionValidate();
				if (validate)
					prop->setValue(getClearPositionValue(), mTypeName);

				setColourPosition(validate);
			}
		}
		else
		{
			mPosition->setCaption("");
		}
	}

	bool StatePropertyControl::isPositionValidate()
	{
		MyGUI::UString value = mPosition->getOnlyText();

		MyGUI::IntPoint position;
		if (!MyGUI::utility::parseComplex(value, position.left, position.top))
			return false;

		return true;
	}

	MyGUI::UString StatePropertyControl::getClearPositionValue()
	{
		MyGUI::UString value = mPosition->getOnlyText();
		MyGUI::IntPoint position;
		MyGUI::utility::parseComplex(value, position.left, position.top);
		return position.print();
	}

	void StatePropertyControl::setColourPosition(bool _validate)
	{
		MyGUI::UString value = mPosition->getOnlyText();
		if (!_validate)
			value = "#FF0000" + value;

		size_t index = mPosition->getTextCursor();
		mPosition->setCaption(value);
		mPosition->setTextCursor(index);
	}

} // namespace tools
