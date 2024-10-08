/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldPosition.h"
#include "Localise.h"
#include "UndoManager.h"
#include "Parse.h"
#include "CommandManager.h"

namespace tools
{

	const std::string DEFAULT_STRING = "[DEFAULT]";

	PropertyFieldPosition::PropertyFieldPosition(MyGUI::Widget* _parent) :
		BaseLayout("PropertyFieldPosition.layout", _parent)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");
		assignWidget(mButton, "Button");

		mField->eventEditTextChange += newDelegate(this, &PropertyFieldPosition::notifyTryApplyProperties);
		mField->eventEditSelectAccept += newDelegate(this, &PropertyFieldPosition::notifyForceApplyProperties);

		mButton->eventMouseButtonClick += newDelegate(this, &PropertyFieldPosition::notifyMouseButtonClick);

		CommandManager::getInstance()
			.getEvent("Command_ToggleRelativeMode")
			->connect(this, &PropertyFieldPosition::commandToggleRelativeMode);
		EditorWidgets::getInstance().eventChangeWidgetCoord +=
			MyGUI::newDelegate(this, &PropertyFieldPosition::notifyPropertyChangeCoord);
	}

	PropertyFieldPosition::~PropertyFieldPosition()
	{
		EditorWidgets::getInstance().eventChangeWidgetCoord -=
			MyGUI::newDelegate(this, &PropertyFieldPosition::notifyPropertyChangeCoord);
		CommandManager::getInstance().getEvent("Command_ToggleRelativeMode")->disconnect(this);
	}

	void PropertyFieldPosition::initialise(std::string_view _type)
	{
		mType = _type;
	}

	void PropertyFieldPosition::setTarget(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;

		updateButton();
	}

	void PropertyFieldPosition::notifyApplyProperties(MyGUI::Widget* _sender, bool _force)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		std::string value = mField->getOnlyText();
		if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
			value.clear();

		onAction(value, _force);

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

	void PropertyFieldPosition::onAction(std::string_view _value, bool _force)
	{
		EditorWidgets* ew = &EditorWidgets::getInstance();
		WidgetContainer* widgetContainer = ew->find(mCurrentWidget);

		bool goodData = onCheckValue();

		if (goodData)
		{
			if (widgetContainer->getRelativeMode())
			{
				std::stringstream str;
				str << _value;
				MyGUI::DoubleCoord double_coord;
				str >> double_coord;
				double_coord.left /= 100;
				double_coord.top /= 100;
				double_coord.width /= 100;
				double_coord.height /= 100;
				MyGUI::IntCoord coord =
					MyGUI::CoordConverter::convertFromRelative(double_coord, mCurrentWidget->getParentSize());

				mCurrentWidget->setCoord(coord);
				EditorWidgets::getInstance().onSetWidgetCoord(
					mCurrentWidget,
					mCurrentWidget->getAbsoluteCoord(),
					"PropertiesPanelView");
			}
			else
			{
				widgetContainer->getWidget()->setProperty("Coord", _value);
				EditorWidgets::getInstance().onSetWidgetCoord(
					mCurrentWidget,
					mCurrentWidget->getAbsoluteCoord(),
					"PropertiesPanelView");
			}
		}
	}

	void PropertyFieldPosition::notifyTryApplyProperties(MyGUI::EditBox* _sender)
	{
		notifyApplyProperties(_sender, false);
	}

	void PropertyFieldPosition::notifyForceApplyProperties(MyGUI::EditBox* _sender)
	{
		notifyApplyProperties(_sender, true);
	}

	bool PropertyFieldPosition::onCheckValue()
	{
		bool success = true;

		if (EditorWidgets::getInstance().find(mCurrentWidget)->getRelativeMode())
			success = utility::checkParse<float>(mField, 4);
		else
			success = utility::checkParse<int>(mField, 4);

		return success;
	}

	MyGUI::IntSize PropertyFieldPosition::getContentSize()
	{
		return {0, mMainWidget->getHeight()};
	}

	void PropertyFieldPosition::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

	void PropertyFieldPosition::setValue(std::string_view _value)
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

	void PropertyFieldPosition::setName(std::string_view _value)
	{
		mName = _value;
		mText->setCaption(mName);
	}

	void PropertyFieldPosition::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		notifyToggleRelativeMode();
	}

	void PropertyFieldPosition::commandToggleRelativeMode(const MyGUI::UString& _commandName, bool& _result)
	{
		notifyToggleRelativeMode();

		_result = true;
	}

	void PropertyFieldPosition::notifyToggleRelativeMode()
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		// если нет контейнера, занчит мы не в режиме редактирования
		if (widgetContainer == nullptr)
			return;

		widgetContainer->setRelativeMode(!widgetContainer->getRelativeMode());

		updateButton();
		updatePositionCaption();

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

	void PropertyFieldPosition::updateButton()
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		if (widgetContainer->getRelativeMode())
			mButton->setCaption(replaceTags("to_percents"));
		else
			mButton->setCaption(replaceTags("to_pixels"));
	}

	void PropertyFieldPosition::updatePositionCaption()
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		setValue(widgetContainer->position());
	}

	void PropertyFieldPosition::notifyPropertyChangeCoord(
		MyGUI::Widget* _widget,
		const MyGUI::IntCoord& _coordValue,
		std::string_view _owner)
	{
		if (_owner == "PropertiesPanelView" || _widget != mCurrentWidget)
			return;

		updatePositionCaption();
	}

	void PropertyFieldPosition::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	bool PropertyFieldPosition::getVisible()
	{
		return mMainWidget->getVisible();
	}

}
