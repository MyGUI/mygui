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
	#define ON_EXIT( CODE ) class _OnExit { public: void dummy() const { }; ~_OnExit() { CODE; } } _onExit; _onExit.dummy()

	const std::string DEFAULT_STRING = "[DEFAULT]";

	PropertyFieldEditBox::PropertyFieldEditBox(MyGUI::Widget* _parent) :
		BaseLayout("PropertyFieldEditBox.layout", _parent),
		mText(nullptr),
		mField(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");
		//mText = _window->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(), MyGUI::Align::Default);
		//mText->setTextAlign(MyGUI::Align::Right);

		//mField = _window->createWidget<MyGUI::EditBox>("Edit", MyGUI::IntCoord(), MyGUI::Align::Top | MyGUI::Align::HStretch);
		mField->eventEditTextChange += newDelegate (this, &PropertyFieldEditBox::notifyTryApplyProperties);
		mField->eventEditSelectAccept += newDelegate (this, &PropertyFieldEditBox::notifyForceApplyProperties);
	}

	PropertyFieldEditBox::~PropertyFieldEditBox()
	{
		//destroy();
	}

	void PropertyFieldEditBox::initialise(/*MyGUI::Widget* _window, */const std::string& _type, MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		mType = _type;
	}

	/*void PropertyFieldEditBox::destroy()
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(mText);
		mText = nullptr;
		MyGUI::WidgetManager::getInstance().destroyWidget(mField);
		mField = nullptr;
	}*/

	void PropertyFieldEditBox::notifyApplyProperties(MyGUI::Widget* _sender, bool _force)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		EditorWidgets* ew = &EditorWidgets::getInstance();
		WidgetContainer* widgetContainer = ew->find(mCurrentWidget);

		std::string value = mField->getOnlyText();

		ON_EXIT(UndoManager::getInstance().addValue(PR_PROPERTIES););

		bool goodData = checkType(mField, mType);

		if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
			value = "";

		if (mName == "Name")
		{
			widgetContainer->name = value;
			ew->invalidateWidgets();
			return;
		}
		else if (mName == "Position")
		{
			if (!goodData)
				return;

			if (widgetContainer->relative_mode)
			{
				std::istringstream str(value);
				MyGUI::FloatCoord float_coord;
				str >> float_coord;
				float_coord.left = float_coord.left / 100;
				float_coord.top = float_coord.top / 100;
				float_coord.width = float_coord.width / 100;
				float_coord.height = float_coord.height / 100;
				MyGUI::IntCoord coord = MyGUI::CoordConverter::convertFromRelative(float_coord, mCurrentWidget->getParentSize());
				mCurrentWidget->setCoord(coord);

				EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");
				return;
			}
			widgetContainer->widget->setProperty("Widget_Coord", value);

			EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");
			return;
		}
		else
		{
			std::string tmp = mName;
			if (splitString(tmp, ' ') == "Controller")
			{
				int n = MyGUI::utility::parseValue<int>(splitString(tmp, ' '));
				std::string key = splitString(tmp, ' ');
				widgetContainer->mController[n]->mProperty[key] = value;
				return;
			}
		}

		bool success = false;
		if (goodData || _force)
			success = ew->tryToApplyProperty(widgetContainer->widget, mName, value);
		else
			return;

		if (success)
		{
			EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");
		}
		else
		{
			mField->setCaption(DEFAULT_VALUE);
			return;
		}

		// если такое св-во было, то заменим (или удалим если стерли) значение
		for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
		{
			if (iterProperty->first == mName)
			{
				if (value.empty())
					widgetContainer->mProperty.erase(iterProperty);
				else
					iterProperty->second = value;
				return;
			}
		}

		// если такого свойства не было раньше, то сохраняем
		if (!value.empty())
			widgetContainer->mProperty.push_back(MyGUI::PairString(mName, value));
	}

	void PropertyFieldEditBox::notifyTryApplyProperties(MyGUI::EditBox* _sender)
	{
		notifyApplyProperties(_sender, false);
	}

	void PropertyFieldEditBox::notifyForceApplyProperties(MyGUI::EditBox* _sender)
	{
		notifyApplyProperties(_sender, true);
	}

	bool PropertyFieldEditBox::checkType(MyGUI::EditBox* _edit, const std::string& _type)
	{
		bool success = true;

		if ("Name" == _type)
		{
			// теперь имя может быть не уникальным
			const MyGUI::UString& text = _edit->getOnlyText();
			size_t index = _edit->getTextCursor();
			_edit->setCaption(text);
			success = true;
			_edit->setTextCursor(index);
		}
		else if ("Position" == _type)
		{
			if (EditorWidgets::getInstance().find(mCurrentWidget)->relative_mode)
				success = utility::checkParse<float>(_edit, 4);
			else
				success = utility::checkParse<int>(_edit, 4);
		}
		else if ("1 int" == _type)
			success = utility::checkParse<int>(_edit, 1);
		else if ("2 int" == _type)
			success = utility::checkParse<int>(_edit, 2);
		else if ("4 int" == _type)
			success = utility::checkParse<int>(_edit, 4);
		else if ("Alpha" == _type)
			success = utility::checkParseInterval<float>(_edit, 1, 0., 1.);
		else if ("1 float" == _type)
			success = utility::checkParse<float>(_edit, 1);
		else if ("2 float" == _type)
			success = utility::checkParse<float>(_edit, 2);
		else if ("FileName" == _type)
			success = utility::checkParseFileName(_edit);
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
		/*int w1 = 120;
		int x2 = 125;

		mText->setCoord(MyGUI::IntCoord(0, _coord.top, w1, _coord.height));
		mField->setCoord(MyGUI::IntCoord(x2, _coord.top, _coord.width - x2, _coord.height));*/
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
			checkType(mField, mType);
		}
	}

	void PropertyFieldEditBox::setName(const std::string& _value)
	{
		mName = _value;
		mText->setCaption(_value);
	}

} // namespace tools
