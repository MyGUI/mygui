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
	#define ON_EXIT( CODE ) class _OnExit { public: void dummy() const { }; ~_OnExit() { CODE; } } _onExit; _onExit.dummy()

	const std::string DEFAULT_STRING = "[DEFAULT]";
	const int BAR_HEIGHT = 30;

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
			mField->addItem(*iter);
		mField->beginToItemFirst();

		mField->setUserString("type", _type);
	}

	void PropertyFieldComboBox::destroy()
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(mText);
		mText = nullptr;
		MyGUI::WidgetManager::getInstance().destroyWidget(mField);
		mField = nullptr;
	}

	void PropertyFieldComboBox::notifyApplyProperties(MyGUI::Widget* _sender, bool _force)
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

		if (action == "Type")
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

	void PropertyFieldComboBox::notifyForceApplyProperties2(MyGUI::ComboBox* _sender, size_t _index)
	{
		notifyApplyProperties(_sender, true);
	}

	void PropertyFieldComboBox::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
	{
		if (_sender->getUserString("type") == "Skin")
		{
			if (_info.type == MyGUI::ToolTipInfo::Show)
			{
				SkinInfo data = getCellData(_sender, _info.index);
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

	SkinInfo PropertyFieldComboBox::getCellData(MyGUI::Widget* _sender, size_t _index)
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

	bool PropertyFieldComboBox::checkType(MyGUI::EditBox* _edit, const std::string& _type)
	{
		bool success = true;
		return success;
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
			mField->castType<MyGUI::EditBox>()->setCaption(DEFAULT_VALUE);
		}
		else
		{
			mField->castType<MyGUI::EditBox>()->setOnlyText(_value);
			checkType(mField->castType<MyGUI::EditBox>(), mType);
		}
	}

	void PropertyFieldComboBox::setName(const std::string& _value)
	{
		mField->setUserString("action", _value);
		mText->setCaption(_value);
	}

} // namespace tools
