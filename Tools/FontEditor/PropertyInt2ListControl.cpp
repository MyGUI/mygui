/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyInt2ListControl.h"
#include "Localise.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(PropertyInt2ListControl)

	PropertyInt2ListControl::PropertyInt2ListControl() :
		mName(nullptr),
		mList(nullptr),
		mAdd(nullptr),
		mDelete(nullptr),
		mTextFieldControl(nullptr)
	{
	}

	PropertyInt2ListControl::~PropertyInt2ListControl()
	{
		mAdd->eventMouseButtonClick -= MyGUI::newDelegate(this, &PropertyInt2ListControl::notifyClickAdd);
		mDelete->eventMouseButtonClick -= MyGUI::newDelegate(this, &PropertyInt2ListControl::notifyClickDelete);

		delete mTextFieldControl;
		mTextFieldControl = nullptr;
	}

	void PropertyInt2ListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		PropertyControl::OnInitialise(_parent, _place, "PropertyListControl.layout");

		assignWidget(mName, "Name", false);
		assignWidget(mList, "List");
		assignWidget(mAdd, "Add");
		assignWidget(mDelete, "Delete");

		mTextFieldControl = new TextFieldControl();
		mTextFieldControl->Initialise();
		mTextFieldControl->eventEndDialog.connect(this, &PropertyInt2ListControl::notifyEndDialog);

		mAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &PropertyInt2ListControl::notifyClickAdd);
		mDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &PropertyInt2ListControl::notifyClickDelete);
	}

	void PropertyInt2ListControl::updateCaption()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
			mName->setCaption(proper->getType()->getName());
	}

	void PropertyInt2ListControl::updateProperty()
	{
		PropertyPtr proper = getProperty();
		if (proper != nullptr)
		{
			mList->setEnabled(!proper->getType()->getReadOnly());
			std::string value = getValue();
			if (value != proper->getValue())
				setValue(proper->getValue());
		}
		else
		{
			mList->removeAllItems();
			mList->setEnabled(false);
		}
	}

	std::string PropertyInt2ListControl::getValue()
	{
		std::string result;

		for (size_t index = 0; index < mList->getItemCount(); index ++)
		{
			if (!result.empty())
				result += "|";
			result += mList->getItemNameAt(index);
		}

		return result;
	}

	void PropertyInt2ListControl::setValue(const std::string& _value)
	{
		mList->removeAllItems();
		std::vector<std::string> values = MyGUI::utility::split(_value, "|");
		for (size_t index = 0; index < values.size(); index ++)
			mList->addItem(values[index]);
	}

	void PropertyInt2ListControl::notifyClickAdd(MyGUI::Widget* _sender)
	{
		mList->addItem("");
		mList->beginToItemAt(mList->getItemCount() - 1);

		mTextFieldControl->setCaption(replaceTags("CaptionAddResource"));
		mTextFieldControl->setTextField("");

		MyGUI::Widget* widget = mList->getWidgetByIndex(mList->getItemCount() - 1);
		if (widget != nullptr)
			mTextFieldControl->setCoord(MyGUI::IntCoord(widget->getAbsoluteLeft(), widget->getAbsoluteTop(), widget->getWidth(), widget->getHeight()));

		mTextFieldControl->doModal();
	}

	void PropertyInt2ListControl::notifyClickDelete(MyGUI::Widget* _sender)
	{
		size_t index = mList->getIndexSelected();
		if (index != MyGUI::ITEM_NONE)
		{
			mList->removeItemAt(index);
			executeAction(getValue());
		}
	}

	void PropertyInt2ListControl::notifyEndDialog(Dialog* _sender, bool _result)
	{
		mTextFieldControl->endModal();

		if (_result)
		{
			if (mTextFieldControl->getTextField() != "")
			{
				mList->setItemNameAt(mList->getItemCount() - 1, mTextFieldControl->getTextField());
				executeAction(getValue());
				return;
			}
		}

		mList->removeItemAt(mList->getItemCount() - 1);
	}

}
