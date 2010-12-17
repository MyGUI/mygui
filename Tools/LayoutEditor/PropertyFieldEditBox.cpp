/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldEditBox.h"
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
	const std::string DEFAULT_STRING = "[DEFAULT]";

	PropertyFieldEditBox::PropertyFieldEditBox(MyGUI::Widget* _parent) :
		BaseLayout("PropertyFieldEditBox.layout", _parent),
		mText(nullptr),
		mField(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		mField->eventEditTextChange += newDelegate (this, &PropertyFieldEditBox::notifyTryApplyProperties);
		mField->eventEditSelectAccept += newDelegate (this, &PropertyFieldEditBox::notifyForceApplyProperties);
	}

	PropertyFieldEditBox::~PropertyFieldEditBox()
	{
	}

	void PropertyFieldEditBox::initialise(const std::string& _type, MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		mType = _type;
	}

	void PropertyFieldEditBox::notifyApplyProperties(MyGUI::Widget* _sender, bool _force)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		std::string value = mField->getOnlyText();
		if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
			value = "";

		onAction(value, _force);

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

	void PropertyFieldEditBox::onAction(const std::string& _value, bool _force)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		EditorWidgets* ew = &EditorWidgets::getInstance();
		WidgetContainer* widgetContainer = ew->find(mCurrentWidget);

		bool goodData = onCheckValue();

		if (mName == "Position")
		{
			if (goodData)
			{
				if (widgetContainer->relative_mode)
				{
					std::istringstream str(_value);
					MyGUI::FloatCoord float_coord;
					str >> float_coord;
					float_coord.left = float_coord.left / 100;
					float_coord.top = float_coord.top / 100;
					float_coord.width = float_coord.width / 100;
					float_coord.height = float_coord.height / 100;
					MyGUI::IntCoord coord = MyGUI::CoordConverter::convertFromRelative(float_coord, mCurrentWidget->getParentSize());
					mCurrentWidget->setCoord(coord);

					EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");
				}
				else
				{
					widgetContainer->widget->setProperty("Coord", _value);
					EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");
				}
			}
		}
		else
		{
			std::string tmp = mName;
			if (splitString(tmp, ' ') == "Controller")
			{
				int n = MyGUI::utility::parseValue<int>(splitString(tmp, ' '));
				std::string key = splitString(tmp, ' ');
				widgetContainer->mController[n]->mProperty[key] = _value;
			}
			else
			{
				if (goodData || _force)
				{
					bool success = ew->tryToApplyProperty(widgetContainer->widget, mName, _value);

					if (success)
					{
						EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");

						bool found = false;
						// если такое св-во было, то заменим (или удалим если стерли) значение
						for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
						{
							if (iterProperty->first == mName)
							{
								found = true;
								if (_value.empty())
									widgetContainer->mProperty.erase(iterProperty);
								else
									iterProperty->second = _value;
							}
						}

						// если такого свойства не было раньше, то сохраняем
						if (!_value.empty() && !found)
							widgetContainer->mProperty.push_back(MyGUI::PairString(mName, _value));
					}
					else
					{
						mField->setCaption(DEFAULT_VALUE);
					}
				}
			}
		}
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

		if ("1 int" == mType)
			success = utility::checkParse<int>(mField, 1);
		else if ("2 int" == mType)
			success = utility::checkParse<int>(mField, 2);
		else if ("4 int" == mType)
			success = utility::checkParse<int>(mField, 4);
		else if ("1 float" == mType)
			success = utility::checkParse<float>(mField, 1);
		else if ("2 float" == mType)
			success = utility::checkParse<float>(mField, 2);
		else if ("Alpha" == mType)
			success = utility::checkParseInterval<float>(mField, 1, 0., 1.);
		else if ("Position" == mType)
		{
			if (EditorWidgets::getInstance().find(mCurrentWidget)->relative_mode)
				success = utility::checkParse<float>(mField, 4);
			else
				success = utility::checkParse<int>(mField, 4);
		}

		return success;
	}

	std::string PropertyFieldEditBox::splitString(std::string& str, char separator)
	{
		size_t spaceIdx = str.find(separator);
		if (spaceIdx == std::string::npos)
		{
			std::string tmp = str;
			str.clear();
			return tmp;
		}
		else
		{
			std::string tmp = str.substr(0, spaceIdx);
			str.erase(0, spaceIdx + 1);
			return tmp;
		}
	}

	MyGUI::IntSize PropertyFieldEditBox::getContentSize()
	{
		return MyGUI::IntSize(0, SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("PropertyItemHeight"));
	}

	void PropertyFieldEditBox::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

	void PropertyFieldEditBox::setValue(const std::string& _value)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		if (_value.empty())
		{
			mField->setCaption(DEFAULT_VALUE);
		}
		else
		{
			mField->setOnlyText(_value);
			onCheckValue();
		}
	}

	void PropertyFieldEditBox::setName(const std::string& _value)
	{
		mName = _value;
		mText->setCaption(_value);
	}

} // namespace tools
