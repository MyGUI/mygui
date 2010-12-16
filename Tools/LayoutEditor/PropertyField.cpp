/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "PropertyField.h"
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
	const int BAR_HEIGHT = 30;

	PropertyField::PropertyField() :
		mText(nullptr),
		mField(nullptr)
	{
	}

	void PropertyField::createPropertiesWidgetsPair(MyGUI::Widget* _window, const std::string& _property, const std::string& _value, const std::string& _type, int y, MyGUI::Widget* _currentWidget, EditorToolTip* _toolTip)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		mCurrentWidget = _currentWidget;
		mToolTip = _toolTip;

		int x1 = 0;
		int x2 = 125;
		int w1 = 120;
		int w2 = _window->getWidth() - x2;
		const int h = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("PropertyItemHeight");

		if (_property == "Position")
		{
			x1 = 66;
			w1 = w1 - x1;
		}

		MyGUI::TextBox* text = nullptr;
		MyGUI::Widget* editOrCombo = nullptr;
		//int string_int_float; // 0 - string, 1 - int, 2 - float

		enum PropertyType
		{
			PropertyType_Edit,
			PropertyType_ComboBox,
			PropertyType_EditAcceptOnly,
			PropertyType_Count
		};

		PropertyType widget_for_type = PropertyType_Count;

		std::string type_names[PropertyType_Count] = { "Edit", "ComboBox", "Edit" };

		if ("Name" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("Type" == _type)
			widget_for_type = PropertyType_ComboBox;
		else if ("Skin" == _type)
			widget_for_type = PropertyType_ComboBox;
		else if ("Position" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("Layer" == _type)
			widget_for_type = PropertyType_ComboBox;
		else if ("String" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("StringAccept" == _type)
			widget_for_type = PropertyType_EditAcceptOnly;
		else if ("Align" == _type)
			widget_for_type = PropertyType_ComboBox;
		else if ("FileName" == _type)
			widget_for_type = PropertyType_Edit;
		// не совсем правильно FIXME
		else if ("1 int" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("2 int" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("4 int" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("alpha" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("1 float" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("2 float" == _type)
			widget_for_type = PropertyType_Edit;
		// надо сделать проще FIXME
		else if ("Colour" == _type)
			widget_for_type = PropertyType_Edit; //"Colour" хорошо бы колорпикером
		else
			widget_for_type = PropertyType_ComboBox;

		text = _window->createWidget<MyGUI::TextBox>("TextBox", x1, y, w1, h, MyGUI::Align::Default);
		text->setTextAlign(MyGUI::Align::Right);

		std::string prop = _property;

		text->setCaption(prop);

		if (widget_for_type == PropertyType_Edit)
		{
			editOrCombo = _window->createWidget<MyGUI::EditBox>("Edit", x2, y, w2, h, MyGUI::Align::Top | MyGUI::Align::HStretch);
			editOrCombo->castType<MyGUI::EditBox>()->eventEditTextChange += newDelegate (this, &PropertyField::notifyTryApplyProperties);
			editOrCombo->castType<MyGUI::EditBox>()->eventEditSelectAccept += newDelegate (this, &PropertyField::notifyForceApplyProperties);
		}
		else if (widget_for_type == PropertyType_ComboBox)
		{
			editOrCombo = _window->createWidget<MyGUI::ComboBox>("ComboBox", x2, y, w2, h, MyGUI::Align::Top | MyGUI::Align::HStretch);
			editOrCombo->castType<MyGUI::ComboBox>()->eventComboAccept += newDelegate (this, &PropertyField::notifyForceApplyProperties2);

			editOrCombo->castType<MyGUI::ComboBox>()->setComboModeDrop(true);

			editOrCombo->setNeedToolTip(true);
			editOrCombo->eventToolTip += newDelegate (this, &PropertyField::notifyToolTip);
		}
		else if (widget_for_type == PropertyType_EditAcceptOnly)
		{
			editOrCombo = _window->createWidget<MyGUI::EditBox>("Edit", x2, y, w2, h, MyGUI::Align::Top | MyGUI::Align::HStretch);
			editOrCombo->castType<MyGUI::EditBox>()->eventEditSelectAccept += newDelegate (this, &PropertyField::notifyForceApplyProperties);
		}

		mText = text;
		mField = editOrCombo;

		// fill possible values
		if (widget_for_type == PropertyType_ComboBox)
		{
			std::vector<std::string> values;
			if (_type == "Skin")
			{
				values = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName())->skin;

				// добавляем скины и шаблоны
				MyGUI::ResourceManager::EnumeratorPtr resource = MyGUI::ResourceManager::getInstance().getEnumerator();
				while (resource.next())
				{
					MyGUI::ResourceSkin* resourceSkin = resource.current().second->castType<MyGUI::ResourceSkin>(false);
					if (resourceSkin != nullptr)
					{
						if (!MyGUI::utility::startWith(resourceSkin->getResourceName(), "LE_"))
							values.push_back(replaceTags("ColourDefault") + resourceSkin->getResourceName());
					}

					MyGUI::ResourceLayout* resourceLayout = resource.current().second->castType<MyGUI::ResourceLayout>(false);
					if (resourceLayout != nullptr)
					{
						if (!MyGUI::utility::endWith(resourceLayout->getResourceName(), ".layout"))
						{
							if (!MyGUI::utility::startWith(resourceLayout->getResourceName(), "LE_"))
								values.push_back(replaceTags("ColourDefault") + resourceLayout->getResourceName());
						}
					}
				}
			}
			else if (_type == "Type")
			{
				VectorWidgetType types = WidgetTypes::getInstance().getWidgetTypes();
				for (VectorWidgetType::iterator iter = types.begin(); iter != types.end(); ++iter)
				{
					values.push_back((*iter)->name);
				}
			}
			else if (_type == "Font")
			{
				values.push_back(replaceTags("ColourDefault") + DEFAULT_STRING);
				values.push_back("Default");

				MyGUI::ResourceManager::EnumeratorPtr resource = MyGUI::ResourceManager::getInstance().getEnumerator();
				while (resource.next())
				{
					MyGUI::IFont* resourceFont = resource.current().second->castType<MyGUI::IFont>(false);
					if (resourceFont != nullptr)
						values.push_back(resourceFont->getResourceName());
				}
			}
			else
			{
				values = WidgetTypes::getInstance().findPossibleValues(_type);
			}

			for (std::vector<std::string>::iterator iter = values.begin(); iter != values.end(); ++iter)
				editOrCombo->castType<MyGUI::ComboBox>()->addItem(*iter);
			editOrCombo->castType<MyGUI::ComboBox>()->beginToItemFirst();
		}

		editOrCombo->setUserString("action", _property);
		editOrCombo->setUserString("type", _type);

		if (_value.empty())
		{
			editOrCombo->castType<MyGUI::EditBox>()->setCaption(DEFAULT_VALUE);
		}
		else
		{
			editOrCombo->castType<MyGUI::EditBox>()->setOnlyText(_value);
			checkType(editOrCombo->castType<MyGUI::EditBox>(), _type);
		}
	}

	void PropertyField::destroy()
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(mText);
		mText = nullptr;
		MyGUI::WidgetManager::getInstance().destroyWidget(mField);
		mField = nullptr;
	}

	void PropertyField::notifyApplyProperties(MyGUI::Widget* _sender, bool _force)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		EditorWidgets* ew = &EditorWidgets::getInstance();
		WidgetContainer* widgetContainer = ew->find(mCurrentWidget);
		MyGUI::EditBox* senderEdit = _sender->castType<MyGUI::EditBox>();
		std::string action = senderEdit->getUserString("action");
		std::string value = senderEdit->getOnlyText();
		std::string type = senderEdit->getUserString("type");

		ON_EXIT(UndoManager::getInstance().addValue(PR_PROPERTIES););

		bool goodData = checkType(senderEdit, type);

		if (value == DEFAULT_STRING && senderEdit->getCaption() == DEFAULT_VALUE)
			value = "";

		if (action == "Name")
		{
			widgetContainer->name = value;
			ew->invalidateWidgets();
			return;
		}
		else if (action == "Type")
		{
			WidgetSelectorManager::getInstance().saveSelectedWidget();

			widgetContainer->type = value;

			MyGUI::xml::Document* savedDoc = ew->savexmlDocument();
			ew->clear();
			ew->loadxmlDocument(savedDoc);
			delete savedDoc;

			WidgetSelectorManager::getInstance().restoreSelectedWidget();

			return;
		}
		else if (action == "Skin")
		{
			widgetContainer->skin = value;
			if (isSkinExist(widgetContainer->skin) || widgetContainer->skin.empty())
			{
				WidgetSelectorManager::getInstance().saveSelectedWidget();

				MyGUI::xml::Document* savedDoc = ew->savexmlDocument();
				ew->clear();
				ew->loadxmlDocument(savedDoc);
				delete savedDoc;

				WidgetSelectorManager::getInstance().restoreSelectedWidget();
			}
			else
			{
				std::string mess = MyGUI::utility::toString("Skin '", widgetContainer->skin, "' not found. This value will be saved.");
				GroupMessage::getInstance().addMessage(mess, MyGUI::LogLevel::Error);
			}
			return;
		}
		else if (action == "Position")
		{
			if (!goodData) return;
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
		else if (action == "Align")
		{
			widgetContainer->align = value;
			widgetContainer->widget->setAlign(MyGUI::Align::parse(value));
			return;
		}
		else if (action == "Layer")
		{
			widgetContainer->setLayerName(value);
			return;
		}
		else
		{
			std::string tmp = action;
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
			success = ew->tryToApplyProperty(widgetContainer->widget, action, value);
		else
			return;

		if (success)
		{
			EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");
		}
		else
		{
			senderEdit->setCaption(DEFAULT_VALUE);
			return;
		}

		// если такое св-во было, то заменим (или удалим если стерли) значение
		for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
		{
			if (iterProperty->first == action)
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
			widgetContainer->mProperty.push_back(MyGUI::PairString(action, value));
	}

	void PropertyField::notifyTryApplyProperties(MyGUI::EditBox* _sender)
	{
		notifyApplyProperties(_sender, false);
	}

	void PropertyField::notifyForceApplyProperties(MyGUI::EditBox* _sender)
	{
		notifyApplyProperties(_sender, true);
	}

	void PropertyField::notifyForceApplyProperties2(MyGUI::ComboBox* _sender, size_t _index)
	{
		notifyApplyProperties(_sender, true);
	}

	void PropertyField::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
	{
		if (mToolTip == nullptr)
			return;

		if (_sender->getUserString("type") == "Skin")
		{
			if (_info.type == MyGUI::ToolTipInfo::Show)
			{
				SkinInfo data = getCellData(_sender, _info.index);
				mToolTip->show(data);
				mToolTip->move(_info.point);
			}
			else if (_info.type == MyGUI::ToolTipInfo::Hide)
			{
				mToolTip->hide();
			}
			else if (_info.type == MyGUI::ToolTipInfo::Move)
			{
				mToolTip->move(_info.point);
			}
		}
	}

	SkinInfo PropertyField::getCellData(MyGUI::Widget* _sender, size_t _index)
	{
		MyGUI::ComboBox* box = _sender->castType<MyGUI::ComboBox>();
		if (_index != MyGUI::ITEM_NONE)
		{
			MyGUI::UString name = box->getItemNameAt(_index);
			return SkinInfo(MyGUI::TextIterator::getOnlyText(name), "", "");
		}
		else
		{
			MyGUI::UString name = box->getCaption();
			return SkinInfo(MyGUI::TextIterator::getOnlyText(name), "", "");
		}
	}

	bool PropertyField::checkType(MyGUI::EditBox* _edit, const std::string& _type)
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
		else if ("alpha" == _type)
			success = utility::checkParseInterval<float>(_edit, 1, 0., 1.);
		else if ("1 float" == _type)
			success = utility::checkParse<float>(_edit, 1);
		else if ("2 float" == _type)
			success = utility::checkParse<float>(_edit, 2);
		else if ("FileName" == _type)
			success = utility::checkParseFileName(_edit);
		return success;
	}

	bool PropertyField::isSkinExist(const std::string& _skinName)
	{
		return _skinName == "Default" ||
			MyGUI::SkinManager::getInstance().isExist(_skinName) ||
			(MyGUI::LayoutManager::getInstance().isExist(_skinName) && checkTemplate(_skinName));
	}

	std::string PropertyField::splitString(std::string& str, char separator)
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

	bool PropertyField::checkTemplate(const std::string& _skinName)
	{
		MyGUI::ResourceLayout* templateInfo = MyGUI::LayoutManager::getInstance().getByName(_skinName, false);
		if (templateInfo != nullptr)
		{
			const MyGUI::VectorWidgetInfo& data = templateInfo->getLayoutData();
			for (MyGUI::VectorWidgetInfo::const_iterator container = data.begin(); container != data.end(); ++container)
			{
				if (container->name == "Root")
					return true;
			}
		}

		return false;
	}

	MyGUI::EditBox* PropertyField::getField()
	{
		return mField->castType<MyGUI::EditBox>();
	}

} // namespace tools
