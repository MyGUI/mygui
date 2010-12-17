/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldComboBox.h"
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

	PropertyFieldComboBox::PropertyFieldComboBox() :
		mText(nullptr),
		mField(nullptr)
	{
	}

	PropertyFieldComboBox::~PropertyFieldComboBox()
	{
		destroy();
	}

	void PropertyFieldComboBox::initialise(MyGUI::Widget* _window, const std::string& _type, MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		mType = _type;

		mText = _window->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(), MyGUI::Align::Default);
		mText->setTextAlign(MyGUI::Align::Right);

		mField = _window->createWidget<MyGUI::ComboBox>("ComboBox", MyGUI::IntCoord(), MyGUI::Align::Top | MyGUI::Align::HStretch);
		mField->eventComboAccept += newDelegate (this, &PropertyFieldComboBox::notifyForceApplyProperties2);

		mField->setComboModeDrop(true);

		mField->setNeedToolTip(true);
		mField->eventToolTip += newDelegate (this, &PropertyFieldComboBox::notifyToolTip);

		onFillValues();
	}

	void PropertyFieldComboBox::onFillValues()
	{
		std::vector<std::string> values;
		if (mType == "Skin")
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
		else if (mType == "Type")
		{
			VectorWidgetType types = WidgetTypes::getInstance().getWidgetTypes();
			for (VectorWidgetType::iterator iter = types.begin(); iter != types.end(); ++iter)
			{
				values.push_back((*iter)->name);
			}
		}
		else if (mType == "Font")
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
			values = WidgetTypes::getInstance().findPossibleValues(mType);
		}

		for (std::vector<std::string>::iterator iter = values.begin(); iter != values.end(); ++iter)
			mField->addItem(*iter);
		mField->beginToItemFirst();
	}

	void PropertyFieldComboBox::destroy()
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(mText);
		mText = nullptr;
		MyGUI::WidgetManager::getInstance().destroyWidget(mField);
		mField = nullptr;
	}

	void PropertyFieldComboBox::notifyApplyProperties(MyGUI::Widget* _sender)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;
		std::string value = mField->getOnlyText();

		if (value == DEFAULT_STRING && mField->getCaption() == DEFAULT_VALUE)
			value = "";

		onAction(value);
	}

	void PropertyFieldComboBox::onAction(const std::string& _value)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		std::string action = mField->getUserString("action");

		if (action == "Type")
		{
			WidgetSelectorManager::getInstance().saveSelectedWidget();

			widgetContainer->type = _value;

			MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
			EditorWidgets::getInstance().clear();
			EditorWidgets::getInstance().loadxmlDocument(savedDoc);
			delete savedDoc;

			WidgetSelectorManager::getInstance().restoreSelectedWidget();
		}
		else if (action == "Skin")
		{
			widgetContainer->skin = _value;
			if (isSkinExist(widgetContainer->skin) || widgetContainer->skin.empty())
			{
				WidgetSelectorManager::getInstance().saveSelectedWidget();

				MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
				EditorWidgets::getInstance().clear();
				EditorWidgets::getInstance().loadxmlDocument(savedDoc);
				delete savedDoc;

				WidgetSelectorManager::getInstance().restoreSelectedWidget();
			}
			else
			{
				std::string mess = MyGUI::utility::toString("Skin '", widgetContainer->skin, "' not found. This value will be saved.");
				GroupMessage::getInstance().addMessage(mess, MyGUI::LogLevel::Error);
			}
		}
		else if (action == "Align")
		{
			widgetContainer->align = _value;
			widgetContainer->widget->setAlign(MyGUI::Align::parse(_value));
		}
		else if (action == "Layer")
		{
			widgetContainer->setLayerName(_value);
		}
		else
		{
			bool success = EditorWidgets::getInstance().tryToApplyProperty(widgetContainer->widget, action, _value);

			bool found = false;
			// если такое св-во было, то заменим (или удалим если стерли) значение
			for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
			{
				if (iterProperty->first == action)
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
				widgetContainer->mProperty.push_back(MyGUI::PairString(action, _value));
		}

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

	void PropertyFieldComboBox::notifyForceApplyProperties2(MyGUI::ComboBox* _sender, size_t _index)
	{
		notifyApplyProperties(_sender);
	}

	void PropertyFieldComboBox::onToolTip(const MyGUI::ToolTipInfo& _info)
	{
		if (mType == "Skin")
		{
			if (_info.type == MyGUI::ToolTipInfo::Show)
			{
				SkinInfo data = getCellData(_info.index);
				EditorToolTip::getInstancePtr()->show(data);
				EditorToolTip::getInstancePtr()->move(_info.point);
			}
			else if (_info.type == MyGUI::ToolTipInfo::Hide)
			{
				EditorToolTip::getInstancePtr()->hide();
			}
			else if (_info.type == MyGUI::ToolTipInfo::Move)
			{
				EditorToolTip::getInstancePtr()->move(_info.point);
			}
		}
	}

	void PropertyFieldComboBox::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
	{
		onToolTip(_info);
	}

	SkinInfo PropertyFieldComboBox::getCellData(size_t _index)
	{
		if (_index != MyGUI::ITEM_NONE)
		{
			MyGUI::UString name = mField->getItemNameAt(_index);
			return SkinInfo(MyGUI::TextIterator::getOnlyText(name), "", "");
		}
		else
		{
			MyGUI::UString name = mField->getCaption();
			return SkinInfo(MyGUI::TextIterator::getOnlyText(name), "", "");
		}
	}

	bool PropertyFieldComboBox::isSkinExist(const std::string& _skinName)
	{
		return _skinName == "Default" ||
			MyGUI::SkinManager::getInstance().isExist(_skinName) ||
			(MyGUI::LayoutManager::getInstance().isExist(_skinName) && checkTemplate(_skinName));
	}

	bool PropertyFieldComboBox::checkTemplate(const std::string& _skinName)
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

	MyGUI::IntSize PropertyFieldComboBox::getContentSize()
	{
		return MyGUI::IntSize(0, SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("PropertyItemHeight"));
	}

	void PropertyFieldComboBox::setCoord(const MyGUI::IntCoord& _coord)
	{
		int w1 = 120;
		int x2 = 125;

		mText->setCoord(MyGUI::IntCoord(0, _coord.top, w1, _coord.height));
		mField->setCoord(MyGUI::IntCoord(x2, _coord.top, _coord.width - x2, _coord.height));
	}

	void PropertyFieldComboBox::setValue(const std::string& _value)
	{
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		if (_value.empty())
		{
			mField->setCaption(DEFAULT_VALUE);
		}
		else
		{
			mField->setOnlyText(_value);
		}
	}

	void PropertyFieldComboBox::setName(const std::string& _value)
	{
		mField->setUserString("action", _value);
		mText->setCaption(_value);
	}

} // namespace tools
